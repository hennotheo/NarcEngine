#include "render_graph/RenderGraph.h"

#include "renderer/FrameHandler.h"
#include "models/Renderer.h"
#include "platform/vulkan/SwapChain.h"

#include "render_graph/contexts/RenderContext.h"
#include "render_graph/RenderNode.h"
#include "buffers/UniformBuffer.h"

#include "models/Material.h"
#include "resources/Texture2DResource.h"

#include "Engine.h"

namespace narc_engine
{
    void updateDescriptorSet(const Material* mat, const VkDescriptorSet descriptorSet, const VkDescriptorBufferInfo* uniformBuffersInfo)
    {
#warning TODO: remove this temporary code, this is just for testing purposes
        ResourceId textureId = mat->getMainTexture();
        const Texture2DResource* texture = dynamic_cast<const Texture2DResource*>(Engine::getInstance()->resourceManager()->getResource(textureId));

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture->getImageView();
        imageInfo.sampler = texture->getSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSet;
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = uniformBuffersInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSet;
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(Engine::getInstance()->getDevice()->getLogicalDevice()->get(), static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(), 0, nullptr);
    }

    RenderGraph::~RenderGraph()
    {
#warning TODO: remove this temporary code, this is just for testing purposes
        for (auto& node : m_nodes)
        {
            delete node;
            node = nullptr;
        }
        m_nodes.clear();
    }

    UniformBufferObject RenderGraph::updateUniformBuffer(UniformBuffer* buffer, std::vector<const Renderer*>& renderers) const
    {
#warning TODO: remove this temporary code, this is just for testing purposes
        UniformBufferObject ubo{};
        uint16_t maxObjCount = glm::min((uint16_t)renderers.size(), UNIFORM_BUFFER_OBJECT_MAX_INSTANCES);
        for (uint16_t i = 0; i < maxObjCount; i++)
        {
            const Renderer* renderer = renderers[i];
            ubo.Model[i] = renderer->getModelMatrix();

            glm::vec3 worldPosition = glm::vec3(renderer->getModelMatrix()[3]);
            glm::vec3 worldRotation = glm::eulerAngles(glm::quat_cast(renderer->getModelMatrix()));
        }

        ubo.View = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.Proj = glm::perspective(glm::radians(45.0f),
            m_swapchain->getSwapChainExtent().width / (float)m_swapchain->getSwapChainExtent().
            height, 0.1f, 10.0f);//TODO change to target size
        ubo.Proj[1][1] *= -1;

        return ubo;
    }

    void RenderGraph::addNode(RenderNode* node)
    {
        m_nodes.push_back(node);
    }

    void RenderGraph::removeNode(RenderNode* node)
    {
        auto iterator = std::remove(m_nodes.begin(), m_nodes.end(), node);
        if (iterator == m_nodes.end())
        {
            NARCLOG_DEBUG("RenderGraph::removeNode: Node not found in the graph.");
        }
        m_nodes.erase(iterator, m_nodes.end());
    }

    void RenderGraph::buildGraph()
    {
        std::sort(m_nodes.begin(), m_nodes.end(), [](const RenderNode* a, const RenderNode* b) {
            return a->getPriority() < b->getPriority();
            });

        std::set<ResourceId> uniqueResources{};
        for (const auto& node : m_nodes)
        {
            if (!checkIfInputsAreAvailable(node, uniqueResources))
            {
                NARCLOG_FATAL("RenderGraph::buildGraph: Node inputs are not available!");
            }

            for (const auto& output : node->getOutputs())
            {
                uniqueResources.insert(output);
            }
        }
    }

    SignalSemaphores RenderGraph::executeGraph(const FrameHandler* frameHandler, uint32_t imageIndex)
    {
        allocateResources(frameHandler);

        CommandBuffer* bufferForObjects = frameHandler->getCommandPool()->getCommandBuffer(0);
        bufferForObjects->reset(0);

        recordCommandBuffer(bufferForObjects, frameHandler, imageIndex);

        const std::array<VkCommandBuffer, 1> commandBuffers = { bufferForObjects->getVkCommandBuffer() };
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        const VkSemaphore waitSemaphores[] =
        {
            frameHandler->getImageAvailableSemaphore()->get()
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

        const std::vector<VkSemaphore> signalSemaphores = { frameHandler->getRenderFinishedSemaphore()->get() };
        submitInfo.signalSemaphoreCount = signalSemaphores.size();
        submitInfo.pSignalSemaphores = signalSemaphores.data();

        const GraphicsQueue* graphicsQueue = Engine::getInstance()->getGraphicsQueue();
        if (graphicsQueue->submit(1, &submitInfo, frameHandler->getInFlightFence()->get()) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to submit draw command buffer!");
        }

        return signalSemaphores;
    }

    void RenderGraph::allocateResources(const FrameHandler* frameHandler)
    {
        std::set<ResourceId> uniqueMaterials{};
        for (const auto& renderer : m_renderers)
        {
            uniqueMaterials.insert(renderer->getMaterial());
        }

        //UNIFORM BUFFER ALLOCATION
        UniformBuffer* uniformBuffer = frameHandler->getUniformBuffer();

        VkDeviceSize bufferSize = 0;
        for (const auto& material : uniqueMaterials)
        {
            VkDeviceSize size = sizeof(UniformBufferObject);
            bufferSize += uniformBuffer->getValidUniformBufferSize(size);
        }

        uniformBuffer->beginRegister(bufferSize);

        for (const auto& materialId : uniqueMaterials)
        {
            std::vector<const Renderer*> matchingRenderers;
            for (const auto& renderer : m_renderers)
            {
                if (renderer->getMaterial() == materialId)
                {
                    matchingRenderers.push_back(renderer);
                }
            }
            UniformBufferObject ubo = updateUniformBuffer(uniformBuffer, matchingRenderers);
            uniformBuffer->registerBufferObject(&ubo, sizeof(UniformBufferObject));
        }

        uniformBuffer->endRegister();

        uint32_t drawId = 0;
        VkDeviceSize offset = 0;
        for (const auto& materialId : uniqueMaterials)
        {
            VkDeviceSize size = uniformBuffer->getUniformBufferSize(drawId);

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffer->getBuffer();
            bufferInfo.offset = offset;
            bufferInfo.range = size;

            VkDescriptorSet descriptorSet = frameHandler->getDescriptorSet(materialId);
            const Material* mat = NARC_GET_RESOURCE_BY_ID(const Material*, materialId);
            updateDescriptorSet(mat, descriptorSet, &bufferInfo);

            drawId++;
            offset += size;
        }
    }

    void RenderGraph::recordCommandBuffer(CommandBuffer* commandBuffer, const FrameHandler* frameHandler, const uint32_t imageIndex)
    {
        RenderContext ctx{};
        fillRenderContext(&ctx, frameHandler);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        commandBuffer->begin(beginInfo);

        //Temp code, this should be done in the render node
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_swapchain->getRenderPass()->get();
        renderPassInfo.framebuffer = m_swapchain->getFrameBuffer(imageIndex);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_swapchain->getSwapChainExtent();//TODO change to target size

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        commandBuffer->cmdBeginRenderPass(&renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        for (const auto& node : m_nodes)
        {
            node->record(commandBuffer, &ctx);
        }

        commandBuffer->cmdEndRenderPass();

        if (commandBuffer->end() != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to record command buffer!");
        }
    }

    void RenderGraph::fillRenderContext(RenderContext* ctx, const FrameHandler* frameHandler)
    {
        ctx->SwapChainExtent = m_swapchain->getSwapChainExtent();
        ctx->Renderers = &m_renderers;
        ctx->RenderersCount = static_cast<uint32_t>(m_renderers.size());
        ctx->FrameHandler = frameHandler;
    }

    bool RenderGraph::checkIfInputsAreAvailable(const RenderNode* node, const std::set<ResourceId>& availableResources) const
    {
        for (const auto& input : node->getInputs())
        {
            if (availableResources.find(input) == availableResources.end())
            {
                return false;
            }
        }

        return true;
    }
} // namespace narc_engine