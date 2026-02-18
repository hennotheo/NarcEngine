#ifndef NARC_TEST_BUILD

#include <NarcLog.h>
#include <Rhi.h>


// #include "Ubo.h"
// #include "SurfaceManager.h"
// #include "TestDeviceExtension.h"
// #include "layers/VulkanValidationLogger.h"


int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    try
    {
        const auto graphicsInstance = narc_engine::createGraphicsInstance(narc_engine::Vulkan);
        graphicsInstance->setApplicationInfo({
                .ApplicationName = "NarcEngine Editor",
                .EngineName = "NarcEngine"
        });
        graphicsInstance->setDeviceCriteria({
                .RequireGeometryShader = false,
                .RequireDiscreteGPU = false,
                .DeviceRequiredExtensions = {},
                .PreferDiscreteGPU = true
        });

        graphicsInstance->init();

        graphicsInstance->shutdown();
    }
    catch (const std::exception& e)
    {
        NARC_LOG_ERROR("Exception caught: {}", e.what());
    }
}

#endif
