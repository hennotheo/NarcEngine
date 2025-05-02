#include "renderer/EngineRenderer.h"

#include <NarcLog.h>
#include <NarcMath.h>

#include "Engine.h"
#include "gui/UiRenderer.h"
#include "core/interfaces/ISurfaceProvider.h"

namespace narc_engine {
    EngineRenderer::EngineRenderer(const EngineInstance* instance, ISurfaceProvider* surfaceProvider, MultiFrameManager* multiFrameManager) : DeviceComponent()
    {
        m_frameManager = multiFrameManager;

        surfaceProvider->attach(this);

        m_swapChain.create(surfaceProvider);
        createDescriptorSetLayout();
        m_swapChain.createFramebuffers();

        m_uiRenderer = std::make_unique<narc_gui::UiRenderer>(instance, multiFrameManager, &m_swapChain, surfaceProvider);
        NARCLOG_WARNING("Gui renderer must be independent from the engine renderer!");
    }

    EngineRenderer::~EngineRenderer()
    {
        m_swapChain.cleanSwapChain();

        vkDestroyDescriptorSetLayout(getVkDevice(), m_descriptorSetLayout, nullptr);

        for (auto& [id, rendererTask] : m_rendererTasks)
        {
            if (rendererTask != nullptr)
            {
                delete rendererTask;
                rendererTask = nullptr;
            }
        }

        m_swapChain.cleanRenderPass();
    }

    void EngineRenderer::prepareFrame(const FrameHandler* frameHandler)
    {
        m_swapChain.acquireNextImage(frameHandler->getImageAvailableSemaphore(), &m_currentImageIndex);

        uint32_t materialID = 0;
        for (const auto& [id, rendererTask] : m_rendererTasks)
        {
            updateUniformBuffer(frameHandler->getUniformBuffer(), rendererTask);

            rendererTask->updateDescriptorSet(frameHandler->getDescriptorSets()[materialID], frameHandler->getUniformBuffer());
            materialID++;
        }
    }

    SignalSemaphores EngineRenderer::drawFrame(const FrameHandler* frameHandler)
    {
        CommandBuffer* bufferForObjects = frameHandler->getCommandPool()->getCommandBuffer(0);
        bufferForObjects->reset(0);

        const std::array<VkCommandBuffer, 1> commandBuffers = { bufferForObjects->getVkCommandBuffer() };
        recordCommandBuffer(bufferForObjects, m_currentImageIndex, frameHandler->getDescriptorSets());

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        const VkSemaphore waitSemaphores[] =
        {
            frameHandler->getImageAvailableSemaphore()
        };
        constexpr VkPipelineStageFlags waitStages[] =
        {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = commandBuffers.size();
        submitInfo.pCommandBuffers = commandBuffers.data();

        const std::vector<VkSemaphore> signalSemaphores = { frameHandler->getRenderFinishedSemaphore() };
        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();

        const GraphicsQueue* graphicsQueue = Engine::getInstance()->getGraphicsQueue();
        if (graphicsQueue->submit(1, &submitInfo, frameHandler->getInFlightFence()) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to submit draw command buffer!");
        }

        return signalSemaphores;
    }

    void EngineRenderer::presentFrame(SignalSemaphores& signalSemaphores)
    {
        const VkSwapchainKHR swapChains[] = { m_swapChain.getSwapChain() };
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores.data();
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &m_currentImageIndex;
        presentInfo.pResults = nullptr;

        const VkResult result = Engine::getInstance()->getPresentQueue()->presentKHR(&presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized)
        {
            m_framebufferResized = false;
            m_swapChain.reCreate();
        }
        else if (result != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to present swap chain image!");
        }
    }

    void EngineRenderer::updateUniformBuffer(UniformBuffer* buffer, RenderTask* rendererTask) const
    {
        const Renderer* renderer = rendererTask->getRenderers()->data()[0];//TODO change to multiobject
        UniformBufferObject ubo{};
        ubo.Model = renderer->getModelMatrix();
        ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.Proj = glm::perspective(glm::radians(45.0f),
            m_swapChain.getSwapChainExtent().width / (float)m_swapChain.getSwapChainExtent().
            height, 0.1f, 10.0f);
        ubo.Proj[1][1] *= -1;

        buffer->setData(ubo);
    }

    void EngineRenderer::attachRenderer(const Renderer* renderer)
    {
        //TODO change to multimaterial renderer task
        const uint32_t materialID = renderer->getMaterial()->getMaterialID();
        RenderTask* renderTask = m_rendererTasks.contains(materialID)
            ? m_rendererTasks[materialID]
            : createRenderTask(renderer->getMaterial());

        renderTask->bindRenderer(renderer);
    }

    void EngineRenderer::createDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(getVkDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout) !=
            VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create descriptor set layout!");
        }
    }

    void EngineRenderer::recordCommandBuffer(CommandBuffer* commandBuffer, const uint32_t imageIndex, const std::vector<VkDescriptorSet>& descriptorSets)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        commandBuffer->begin(beginInfo);

        VkRenderPassBeginInfo renderPassInfo = m_swapChain.getRenderPassBeginInfos(imageIndex);
        const VkExtent2D swapChainExtent = m_swapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        commandBuffer->cmdBeginRenderPass(&renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        commandBuffer->cmdSetViewport(&viewport, 0, 1);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;
        commandBuffer->cmdSetScissor(&scissor, 0, 1);

        uint32_t materialID = 0;
        for (const auto& [id, rendererTask] : m_rendererTasks)
        {
            rendererTask->recordTask(commandBuffer, &descriptorSets[materialID]);
            materialID++;
        }

        m_uiRenderer->beginFrame();
        m_uiRenderer->render(commandBuffer);//TODO CHANGE TO CUSTOM RENDER PASS

        commandBuffer->cmdEndRenderPass();

        if (commandBuffer->end() != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to record command buffer!");
        }
    }

    RenderTask* EngineRenderer::createRenderTask(const Material* material)
    {
        if (m_rendererTasks.contains(material->getMaterialID()))
        {
            NARCLOG_FATAL("Can't create render task for the same material twice!");
        }

        constexpr uint32_t descriptorSetCount = 1;
        const std::vector<VkDescriptorSetLayout> layouts(descriptorSetCount, m_descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = descriptorSetCount;
        allocInfo.pSetLayouts = layouts.data();

        m_frameManager->allocateDescriptorSets(allocInfo);

        RenderTask* renderer = new RenderTask(&m_swapChain, &m_descriptorSetLayout, material);
        m_rendererTasks.emplace(material->getMaterialID(), renderer);

        return renderer;
    }

    void EngineRenderer::onSurfaceFramebufferResized(int width, int height)
    {
        m_framebufferResized = true;
    }
} // narc_engine