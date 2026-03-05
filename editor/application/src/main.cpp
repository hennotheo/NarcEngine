#ifndef NARC_TEST_BUILD

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

std::unique_ptr<narc_engine::IGraphicsInstance> graphicsInstance = nullptr;
std::unique_ptr<narc_engine::ICommandBufferPool> cmdPool = nullptr;

void stageAndCopyBuffer(const narc_engine::IBuffer* buffer, narc_engine::MemorySize size, const void* data)
{
    narc_engine::BufferAllocationInfo stagingBufferInfo{};
    stagingBufferInfo.IsStaging = true;
    stagingBufferInfo.Size = size;
    const auto stagingBuffer = graphicsInstance->createBuffer(stagingBufferInfo);

    stagingBuffer->setData(data);
    const auto result = cmdPool->allocateOneTimeBuffer();
    if (!result.has_value())
    {
        NARC_ERROR_RUNTIME("CMD Buffer allocation Failed");
    }
    const auto& cmd = result.value();
    cmd->begin();

    cmd->copyBuffer(stagingBuffer.get(), buffer, size);

    cmd->end();

    graphicsInstance->getGraphicsQueue()->submit(
    {
        .CommandBuffers = { cmd.get() }
    });

    graphicsInstance->waitIdle();
}

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    narc_log::init_signal_handling();

    try
    {
        const auto window = narc_engine::createWindow(narc_engine::Glfw);
        window->setTitle("NarcEngine Editor");
        window->init();

        graphicsInstance = narc_engine::createGraphicsInstance(narc_engine::Vulkan);
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
        const auto descriptorSetLayout = graphicsInstance->createDescriptorLayout();
        descriptorSetLayout->addBinding({
                .BindingIndex = 0,
                .Stage = narc_engine::VertexStage,
                .Type = narc_engine::UniformBuffer
        });
        descriptorSetLayout->addBinding({
                .BindingIndex = 1,
                .Stage = narc_engine::FragmentStage,
                .Type = narc_engine::Sampler
        });

        const auto pipelineLayout = graphicsInstance->createPipelineLayout(swapChain.get());
        const auto pipeline = graphicsInstance->createPipeline(pipelineLayout.get(), swapChain.get());
        cmdPool = graphicsInstance->createCommandBufferPool();

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

        narc_engine::VertexLayout vertexLayout{};
        vertexLayout.Stride = sizeof(narc_engine::Vertex);
        vertexLayout.Attributes = std::vector<narc_engine::VertexAttribute>(3);
        vertexLayout.Attributes[0] = narc_engine::VertexAttribute{
                .Location = 0,
                .Binding = 0,
                .Format = narc_engine::Float2,
                .Offset = offsetof(narc_engine::Vertex, pos)
        };
        vertexLayout.Attributes[1] = narc_engine::VertexAttribute{
                .Location = 1,
                .Binding = 0,
                .Format = narc_engine::Float3,
                .Offset = offsetof(narc_engine::Vertex, color)
        };
        vertexLayout.Attributes[2] = narc_engine::VertexAttribute{
                .Location = 2,
                .Binding = 0,
                .Format = narc_engine::Float2,
                .Offset = offsetof(narc_engine::Vertex, texCoord)
        };

        descriptorSetLayout->init();
        pipelineLayout
                ->setVertexLayout(vertexLayout)
                ->setVertexShader("shaders/shader_vert.spv")
                ->setFragmentShader("shaders/shader_frag.spv")
                ->addBinding(descriptorSetLayout.get())
                ->init();
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

        {
            //Buffer lifetime
            const auto verticesSize = narc_engine::s_vertices.size() * sizeof(narc_engine::s_vertices[0]);
            const auto indicesSize = narc_engine::s_indices.size() * sizeof(narc_engine::s_indices[0]);

            narc_engine::BufferAllocationInfo vertexBufferInfo{};
            vertexBufferInfo.IsVertexBuffer = true;
            vertexBufferInfo.Size = verticesSize;
            const auto vertexBuffer = graphicsInstance->createBuffer(vertexBufferInfo);
            stageAndCopyBuffer(vertexBuffer.get(), verticesSize, narc_engine::s_vertices.data());

            narc_engine::BufferAllocationInfo indexBufferInfo{};
            indexBufferInfo.IsIndexBuffer = true;
            indexBufferInfo.Size = indicesSize;
            const auto indexBuffer = graphicsInstance->createBuffer(indexBufferInfo);
            stageAndCopyBuffer(indexBuffer.get(), indicesSize, narc_engine::s_indices.data());

            uint32_t flightInFenceIndex = 0;
            while (!window->shouldClose())
            {
                std::vector<const narc_engine::IFence*> fences = {inFlightFences[flightInFenceIndex].get()};
                graphicsInstance->waitForFences(fences);
                graphicsInstance->resetFences(fences);

                narc_engine::ImageIndex imageIndex = swapChain->acquireNextImage(imageAvailableSemaphores[flightInFenceIndex].get(), nullptr).value();

                auto* cmdBuffer = commandBuffers[flightInFenceIndex].get();
                cmdBuffer->reset();

                //RECORD -------------------------
                cmdBuffer->begin();

                cmdBuffer->beginRenderPass(
                        swapChain.get(),
                        {
                                .TEMPPipeline = pipeline.get(),
                                .TEMPImageIndex = imageIndex
                        });

                cmdBuffer->bindPipeline(pipeline.get());
                cmdBuffer->bindViewPort({
                        .Position = narc_math::Vec2{0, 0},
                        .Dimensions = swapChain->getSwapChainExtent()
                });
                cmdBuffer->bindScissors({
                        .Offset = narc_math::Vec2{0, 0},
                        .Extent = swapChain->getSwapChainExtent()
                });

                cmdBuffer->bindVertexBuffers(vertexBuffer.get());
                cmdBuffer->bindIndexBuffer(indexBuffer.get());

                cmdBuffer->drawIndexed(narc_engine::s_indices.size());

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
                        .WaitSemaphores = {renderFinishedSemaphores[flightInFenceIndex].get()}
                });

                flightInFenceIndex = (flightInFenceIndex + 1) % MAX_FRAMES_IN_FLIGHT;
            }

            graphicsInstance->waitIdle();
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
        descriptorSetLayout->shutdown();
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
