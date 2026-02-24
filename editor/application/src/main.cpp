#ifndef NARC_TEST_BUILD

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    try
    {
        const auto window = narc_engine::createWindow(narc_engine::Glfw);
        window->setTitle("NarcEngine Editor");
        window->init();

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

        graphicsInstance->attachWindow(window.get());
        graphicsInstance->init();

        const auto surface = graphicsInstance->createSurface(window.get());
        const auto swapChain = graphicsInstance->createSwapChain(surface.get());
        surface->init();
        swapChain->init();

        while (!window->shouldClose())
        {
            
        }

        swapChain->shutdown();
        surface->shutdown();

        graphicsInstance->shutdown();

        window->shutdown();
    }
    catch (const std::exception& e)
    {
        NARC_LOG_ERROR("Exception caught: {}", e.what());
    }
}

#endif
