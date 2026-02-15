//
// Created by theo on 12/20/25.
//

#pragma once

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

class SurfaceManager final : public narc_engine::VulkanSurfacesManager
{
public:
    explicit SurfaceManager(std::shared_ptr<narc_core::ICreator<narc_engine::VulkanSwapChain> > swapChainCreator) :
        VulkanSurfacesManager(std::move(swapChainCreator))
    {
    }

    ~SurfaceManager() override = default;

    NO_DISCARD const narc_engine::ISurface* getMainSurface() const noexcept override
    {
        const auto m_surfaces = getSurfaces();
        return m_surfaces.empty() ? nullptr : m_surfaces.front();
    }

    std::vector<std::unique_ptr<narc_engine::VulkanSemaphore> >* imageAvailableSemaphores;
    std::vector<std::unique_ptr<narc_engine::VulkanSemaphore> >* renderFinishedSemaphores;
    std::vector<std::unique_ptr<narc_engine::VulkanFence> >* inFlightFences;
    std::vector<std::unique_ptr<narc_engine::VulkanCommandBuffer> >* cmdBuffer;
    narc_engine::VulkanQueue* graphicsQueue;
    narc_engine::VulkanQueue* presentQueue;
    narc_engine::VulkanVertexBuffer* vertexBuffer;
    narc_engine::VulkanIndexBuffer* indexBuffer;
    narc_engine::VulkanDescriptorSetLayout* descriptor_set_layout;
    std::span<narc_engine::VulkanDescriptorSet> descriptor_sets;
    std::vector<narc_engine::VulkanUniformBuffer>* uniform_buffers;

    uint32_t currentFrame = 0;

    void updateSurfaces() override
    {
        NARC_GUARD_WEAK(device, getDevice(), "Failed to get Vulkan Device.");

        const auto surfaces = getSurfaces();
        const auto swapChains = getSwapChains();
        const auto& framebuffers = getFramebuffers();
        const auto pipelines = getPipeline();
        for (int i = 0; i < surfaces.size(); ++i)
        {
            const auto surf = surfaces[i];
            const auto swapchain = swapChains[i];
            const auto& swapchainFBs = framebuffers[i];
            const auto pipeline = pipelines[i];
            const auto inFlightFence = inFlightFences->at(currentFrame).get();
            const auto imageAvailableSemaphore = imageAvailableSemaphores->at(currentFrame).get();
            const auto renderFinishedSemaphore = renderFinishedSemaphores->at(currentFrame).get();
            const auto cmd = cmdBuffer->at(currentFrame).get();

            inFlightFence->wait();
            inFlightFence->reset();

            uint32_t imageIndex;
            vkAcquireNextImageKHR(device->getHandle(), swapchain->getHandle(), UINT64_MAX, imageAvailableSemaphore->getHandle(), VK_NULL_HANDLE,
                                  &imageIndex);

            updateUniformBuffer(currentFrame, swapchain);

            const auto& framebuffer = swapchainFBs[imageIndex];
            cmd->reset();
            recordCommandBuffer(framebuffer.get(), swapchain, pipeline);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            std::array waitSemaphores = {imageAvailableSemaphore->getHandle()};
            constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = waitSemaphores.size();
            submitInfo.pWaitSemaphores = waitSemaphores.data();
            submitInfo.pWaitDstStageMask = waitStages;

            std::array commandBuffers = {cmd->getHandle()};
            submitInfo.commandBufferCount = commandBuffers.size();
            submitInfo.pCommandBuffers = commandBuffers.data();

            std::array signalSemaphores = {renderFinishedSemaphore->getHandle()};
            submitInfo.signalSemaphoreCount = signalSemaphores.size();
            submitInfo.pSignalSemaphores = signalSemaphores.data();

            if (graphicsQueue->submit(1, submitInfo, inFlightFence) != VK_SUCCESS)
            {
                NARC_ERROR_RUNTIME("Failed to submit draw command buffer!");
            }

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = signalSemaphores.size();
            presentInfo.pWaitSemaphores = signalSemaphores.data();

            VkSwapchainKHR swapChains[] = {swapchain->getHandle()};
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &imageIndex;
            presentInfo.pResults = nullptr; // Optional

            vkQueuePresentKHR(presentQueue->getHandle(), &presentInfo);
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void recordCommandBuffer(const narc_engine::VulkanFramebuffer* framebuffer, const narc_engine::VulkanSwapChain* swapchain,
                             const narc_engine::VulkanGraphicsPipeline* pipeline)
    {
        const auto cmd = cmdBuffer->at(currentFrame).get();

        cmd->begin();
        cmd->beginRenderPass(*framebuffer, *pipeline->getRenderPass());

        cmd->cmdBindPipeline(*pipeline);

        cmd->cmdBindVertexBuffers(*vertexBuffer);
        cmd->cmdBindIndexBuffers(*indexBuffer);

        const auto extend = swapchain->getSwapChainExtent();

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extend.Width);
        viewport.height = static_cast<float>(extend.Height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        cmd->cmdSetViewport(viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = {
                extend.Width,
                extend.Height
        }; //narc_engine::mapping::mapFromExtend(extend);
        cmd->cmdSetScissor(scissor);
        
        std::vector sets = { descriptor_sets[currentFrame] };
        cmd->cmdBindDescriptorSets(sets, *pipeline->getLayout());

        cmd->cmdDrawIndexed(static_cast<uint32_t>(narc_engine::s_indices.size()));

        cmd->endRenderPass();
        cmd->end();
    }

    void updateUniformBuffer(uint32_t currentImage, const narc_engine::VulkanSwapChain* swapchain)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        //TODO: FIX TIME PRECISION OVERFLOW

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), swapchain->getSwapChainExtent().Width / (float) swapchain->getSwapChainExtent().Height, 0.1f,
                                    10.0f);
        ubo.proj[1][1] *= -1;

        uniform_buffers->at(currentImage).setData(sizeof(ubo), &ubo);
    }
};
