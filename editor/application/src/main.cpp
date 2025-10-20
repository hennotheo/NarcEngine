#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    const auto injector = di::make_injector(
            di::bind<narc_engine::IVulkanInstanceConfigProvider>.to<narc_engine::EngineConfigProvider>(),
            di::bind<narc_engine::IVulkanDeviceConfigProvider>.to<narc_engine::EngineConfigProvider>()
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

        instance->init();
        device->init();

        device->shutdown();
        instance->shutdown();
    }
    catch (const std::exception& e)
    {
        NARC_LOG_ERROR("Exception caught: {}", e.what());
    }
}

#endif
