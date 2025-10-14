#include <csignal>
#include <cstdlib>
#include <execinfo.h>
#include <iostream>
#include <unistd.h>

#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    try
    {
        narc_engine::VulkanInstanceInfos instanceInfos{
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine",
        };
        const auto instance = std::make_unique<narc_engine::VulkanInstance>(instanceInfos);

        narc_engine::VulkanDeviceCreationInfos deviceInfos{
                .Instance = *instance
        };
        const auto device = std::make_unique<narc_engine::VulkanDevice>(deviceInfos);

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
