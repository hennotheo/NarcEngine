#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

#include "../../../../../.conan2/p/b/glfwa6e2adfa5e8b8/p/include/GLFW/glfw3.h"

class SurfaceManager final : public narc_engine::VulkanSurfacesManager
{
public:
    explicit SurfaceManager(std::shared_ptr<narc_core::ICreator<narc_engine::VulkanSwapChain>> swapChainCreator) :
        VulkanSurfacesManager(std::move(swapChainCreator))
    {
    }

    ~SurfaceManager() override = default;

    NO_DISCARD const narc_engine::IVulkanSurface* getMainSurface() const noexcept override
    {
        const auto m_surfaces = getSurfaces();
        return m_surfaces.empty() ? nullptr : m_surfaces.front();
    }

    void updateSurfaces() override
    {
        const auto m_surfaces = getSurfaces();
        for (const auto& surface: m_surfaces)
        {
            if (surface->shouldClose())
            {
                NARC_LOG_INFO("Surface requested to close.");
            }
        }
    }
};

class TestDeviceExtensions final : public narc_engine::IVulkanExtension
{
public:
    TestDeviceExtensions() = default;
    ~TestDeviceExtensions() noexcept override = default;

    void init() override
    {
        //Tests
    }

    void shutdown() override
    {
        //Tests
    }

    NO_DISCARD narc_engine::ExtensionNameList getExtensionNames() const noexcept override
    {
        return {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    }

    NO_DISCARD const void* getCreationInfos() const noexcept override
    {
        return nullptr;
    }

};

class Creator final : public narc_core::ICreator<narc_engine::VulkanSwapChain>
{
public:
    std::function<std::unique_ptr<narc_engine::VulkanSwapChain>()> CreateVulkanSwapChain;

    [[nodiscard]] std::unique_ptr<narc_engine::VulkanSwapChain> create() const noexcept override
    {
        return CreateVulkanSwapChain();
    }

};

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    const auto injector = di::make_injector(
            di::bind<narc_engine::IVulkanInstanceConfigProvider>.to<narc_engine::EngineConfigProvider>(),
            di::bind<narc_engine::IVulkanDeviceConfigProvider>.to<narc_engine::EngineConfigProvider>(),
            di::bind<narc_engine::IVulkanSurface>.to<narc_engine::GlfwVulkanSurface>(),
            di::bind<narc_engine::VulkanSurfacesManager>.to<SurfaceManager>(),
            di::bind<narc_core::ICreator<narc_engine::VulkanSwapChain>>.to<Creator>().in(di::singleton)
            );

    {
        const auto creator = injector.create<std::shared_ptr<Creator>>();
        creator->CreateVulkanSwapChain = [&injector] {
            return injector.create<std::unique_ptr<narc_engine::VulkanSwapChain>>();
        };
    }

    try
    {
        {
            const auto configProvider = injector.create<std::weak_ptr<narc_engine::EngineConfigProvider>>();
            NARC_GUARD_WEAK(configProviderPtr, configProvider, "Failed to create VulkanInstanceInfos");
            
            configProviderPtr->m_applicationName = "NarcEngine Editor";
            configProviderPtr->m_engineName = "NarcEngine";
            
            std::vector<std::shared_ptr<narc_engine::IVulkanExtension>> vulkanExtensions;
            vulkanExtensions.push_back(std::make_shared<TestDeviceExtensions>());
            configProviderPtr->m_physicalDeviceCriteria = narc_engine::PhysicalDeviceCriteria{
                    .RequireGeometryShader = false,
                    .RequireDiscreteGPU = false,
                    .DeviceRequiredExtensions = vulkanExtensions,
                    .PreferDiscreteGPU = true
            };
        }


        const auto instance = injector.create<std::shared_ptr<narc_engine::VulkanInstance>>();
        const auto device = injector.create<std::shared_ptr<narc_engine::VulkanDevice>>();
        const auto surfacesManager = injector.create<std::shared_ptr<narc_engine::VulkanSurfacesManager>>();
        surfacesManager->setDevice(device);
        
        auto mainWindow = injector.create<std::unique_ptr<narc_engine::IVulkanSurface>>();
        
        instance->init();
        mainWindow->init();
        surfacesManager->pushSurface(mainWindow);
        device->init();
        surfacesManager->init();
        
        while (!surfacesManager->getMainSurface()->shouldClose())
        {
            glfwPollEvents();
            surfacesManager->updateSurfaces();
        }
        
        surfacesManager->shutdown();
        device->shutdown();
        instance->shutdown();
    }
    catch (const std::exception& e)
    {
        NARC_LOG_ERROR("Exception caught: {}", e.what());
    }
}

#endif
