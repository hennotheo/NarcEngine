#ifndef NARC_TEST_BUILD

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

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
        const auto pipelineLayout = graphicsInstance->createPipelineLayout(swapChain.get());
        const auto pipeline = graphicsInstance->createPipeline(pipelineLayout.get(), swapChain.get());

        std::vector<std::unique_ptr<narc_engine::ISemaphore>> imageAvailableSemaphores;
        std::vector<std::unique_ptr<narc_engine::ISemaphore>> renderFinishedSemaphores;
        std::vector<std::unique_ptr<narc_engine::IFence>> inFlightFences;
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            auto imageAvailableSemaphore = graphicsInstance->createSemaphore();
            auto renderFinishedSemaphore = graphicsInstance->createSemaphore();
            auto inFlightFence = graphicsInstance->createFence();

            imageAvailableSemaphores.push_back(std::move(imageAvailableSemaphore));
            renderFinishedSemaphores.push_back(std::move(renderFinishedSemaphore));
            inFlightFences.push_back(std::move(inFlightFence));
        }



        surface->init();
        swapChain->init();
        pipelineLayout->init();
        pipeline->init();

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            imageAvailableSemaphores[i]->init();
            renderFinishedSemaphores[i]->init();
            inFlightFences[i]->init();
        }

        while (!window->shouldClose())
        {

        }

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            imageAvailableSemaphores[i]->shutdown();
            renderFinishedSemaphores[i]->shutdown();
            inFlightFences[i]->shutdown();
        }

        pipeline->shutdown();
        pipelineLayout->shutdown();
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
