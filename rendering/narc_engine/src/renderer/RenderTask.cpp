#include "renderer/RenderTask.h"

#include "Engine.h"
#include "buffers/GraphicsBuffer.h"
#include "models/ShaderModule.h"
#include "resources/Texture2DResource.h"
#include "models/PushConstants.h"

namespace narc_engine {
    RenderTask::RenderTask(const SwapChain* swapChain, const VkDescriptorSetLayout* descriptorSetLayout,
        const Material* material) : DeviceComponent()
    {
        m_material = material;

        m_pipeline = std::make_unique<GraphicsPipeline>(swapChain, descriptorSetLayout);
    }

    RenderTask::~RenderTask()
    {
    }

    void RenderTask::recordTask(const CommandBuffer* commandBuffer, const VkDescriptorSet* m_descriptorSet) const
    {
        m_pipeline->bindPipeline(commandBuffer);
        commandBuffer->cmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getLayout(), 0, 1,
            m_descriptorSet, 0, nullptr);

        int drawIndex = 0;
        for (const auto renderer : m_renderers)
        {
            PushConstants pushConstants{};
            pushConstants.objectIndex = drawIndex;

            const Mesh* mesh = renderer->getMesh();
            VkBuffer vertexBuffers[] = { mesh->getVertexBuffer()->getBuffer() };
            VkDeviceSize offsets[] = { 0 };
            commandBuffer->cmdBindVertexBuffers(0, 1, vertexBuffers, offsets);
            commandBuffer->cmdBindIndexBuffer(mesh->getIndexBuffer()->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

            commandBuffer->cmdPushConstants(m_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &pushConstants);

            commandBuffer->cmdDrawIndexed(mesh->getIndexCount(), 1, 0, 0, 0);

            drawIndex++;
        }
    }

    void RenderTask::updateDescriptorSet(const VkDescriptorSet descriptorSet, const VkDescriptorBufferInfo* uniformBuffersInfo) const
    {
        GraphicResourceHandler textureHandler = m_material->getMainTexture();
        const Texture2DResource* texture = dynamic_cast<const Texture2DResource*>(Engine::getInstance()->resourceManager()->getResource(textureHandler));
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

        vkUpdateDescriptorSets(getVkDevice(), static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(), 0, nullptr);
    }
} // narc_engine
