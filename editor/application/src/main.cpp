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
        const auto cmdPool = graphicsInstance->createCommandBufferPool();

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

        cmdPool->init();

        std::vector<std::unique_ptr<narc_engine::ICommandBuffer>> commandBuffers;
        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            imageAvailableSemaphores[i]->init();
            renderFinishedSemaphores[i]->init();
            inFlightFences[i]->init();
            commandBuffers.push_back(cmdPool->allocateCommandBuffer().value());
        }

        uint32_t flightInFenceIndex = 0;
        while (!window->shouldClose())
        {
            std::vector<const narc_engine::IFence*> fences = {inFlightFences[flightInFenceIndex].get()};
            graphicsInstance->waitForFences(fences);
            graphicsInstance->resetFences(fences);

            narc_engine::ImageIndex imageIndex = swapChain->acquireNextImage(imageAvailableSemaphores[flightInFenceIndex].get(), nullptr).value();

            auto* cmdBuffer = commandBuffers[imageIndex].get();
            cmdBuffer->reset();

            //RECORD -------------------------
            cmdBuffer->begin();

            cmdBuffer->beginRenderPass(
                    swapChain.get(),
                    {
                            .TEMPPipeline = pipeline.get(),
                            .TEMPFrameInFlightIndex = flightInFenceIndex
                    });

            cmdBuffer->bindPipeline(pipeline.get());
            cmdBuffer->bindScissors({
                .Offset = narc_math::Vec2{0, 0},
                .Extent = swapChain->getSwapChainExtent()
            });
            cmdBuffer->bindViewPort({
                    .Position = narc_math::Vec2{0, 0},
                    .Dimensions = swapChain->getSwapChainExtent()
            });

            cmdBuffer->draw();

            cmdBuffer->endRenderPass();

            cmdBuffer->end();
            //END RECORD ---------------------

            const auto submitQueue = graphicsInstance->getGraphicsQueue();
            submitQueue->submit({
                    .WaitStages = {narc_engine::SubmitWaitStageMask::ColorAttachmentOutput},
                    .CommandBuffers = {cmdBuffer},
                    .SignalSemaphores = {renderFinishedSemaphores[flightInFenceIndex].get()},
                    .WaitSemaphores = {imageAvailableSemaphores[flightInFenceIndex].get()},
                    .Fence = inFlightFences[flightInFenceIndex].get()
            });

            const auto presentQueue = graphicsInstance->getPresentQueue();
            presentQueue->present(
            {
                    .ImageIndices = {imageIndex},
                    .SwapChains = {swapChain.get()},
                    .WaitSemaphores = {imageAvailableSemaphores[flightInFenceIndex].get()}
            });
        }

        for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            imageAvailableSemaphores[i]->shutdown();
            renderFinishedSemaphores[i]->shutdown();
            inFlightFences[i]->shutdown();
        }

        cmdPool->shutdown();

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
