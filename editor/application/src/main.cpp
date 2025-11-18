#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

#include "../../../../../.conan2/p/b/glfwa6e2adfa5e8b8/p/include/GLFW/glfw3.h"

class SurfaceManager final : public narc_engine::IVulkanSurfacesManager
{
public:
    SurfaceManager() = default;
    ~SurfaceManager() override = default;

    void init() override
    {
        for (const auto& surface: m_surfaces)
        {
            surface->init();
        }
    }

    void shutdown() override
    {
        for (const auto& surface: m_surfaces)
        {
            surface->shutdown();
        }
    }

    NO_DISCARD const narc_engine::IVulkanSurface* getMainSurface() const noexcept override
    {
        return m_surfaces.empty() ? nullptr : m_surfaces.front().get();
    }

    void pushSurface(std::unique_ptr<narc_engine::IVulkanSurface> surface) override
    {
        m_surfaces.push_back(std::move(surface));
    }

    void updateSurfaces() override
    {
        for (const auto& surface: m_surfaces)
        {
            if (surface->shouldClose())
            {
                NARC_LOG_INFO("Surface requested to close.");
            }
        }
    }

private:
    std::vector<std::unique_ptr<narc_engine::IVulkanSurface>> m_surfaces{};
};

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    const auto injector = di::make_injector(
            di::bind<narc_engine::IVulkanInstanceConfigProvider>.to<narc_engine::EngineConfigProvider>(),
            di::bind<narc_engine::IVulkanDeviceConfigProvider>.to<narc_engine::EngineConfigProvider>(),
            di::bind<narc_engine::IVulkanSurface>.to<narc_engine::GlfwVulkanSurface>(),
            di::bind<narc_engine::IVulkanSurfacesManager>.to<SurfaceManager>()
            );

    try
    {
        {
            const auto configProvider = injector.create<std::weak_ptr<narc_engine::EngineConfigProvider>>();
            NARC_GUARD_WEAK(configProviderPtr, configProvider, "Failed to create VulkanInstanceInfos");

            configProviderPtr->m_applicationName = "NarcEngine Editor";
            configProviderPtr->m_engineName = "NarcEngine";
            configProviderPtr->m_physicalDeviceCriteria = narc_engine::PhysicalDeviceCriteria{};
        }


        const auto instance = injector.create<std::shared_ptr<narc_engine::VulkanInstance>>();
        const auto device = injector.create<std::shared_ptr<narc_engine::VulkanDevice>>();
        const auto surfacesManager = injector.create<std::shared_ptr<narc_engine::IVulkanSurfacesManager>>();

        auto window = injector.create<std::unique_ptr<narc_engine::IVulkanSurface>>();
        surfacesManager->pushSurface(std::move(window));

        instance->init();
        surfacesManager->init();
        device->init();

        while (!surfacesManager->getMainSurface()->shouldClose())
        {
            glfwPollEvents();
            surfacesManager->updateSurfaces();
        }

        device->shutdown();
        surfacesManager->shutdown();
        instance->shutdown();
    }
    catch (const std::exception& e)
    {
        NARC_LOG_ERROR("Exception caught: {}", e.what());
    }
}

#endif
