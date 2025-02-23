#include "renderer/RenderTask.h"

#include "Engine.h"

namespace narc_engine {
    const std::vector<Vertex> g_vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> g_indices = {
        0, 1, 2, 2, 3, 0
    };

    void RenderTask::create(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout)
    {
        m_device = Engine::getInstance()->getDevice()->getDevice();

        m_graphicsPipeline.create(swapChain, m_descriptorSetLayout);

        m_vertexBuffer.create(g_vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        m_indexBuffer.create(g_indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    }

    void RenderTask::recordTask(const CommandBuffer* commandBuffer, uint32_t currentFrame)
    {
        commandBuffer->cmdBindPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, &m_graphicsPipeline);

        VkBuffer vertexBuffers[] = {m_vertexBuffer.getBuffer()};
        VkDeviceSize offsets[] = {0};
        commandBuffer->cmdBindVertexBuffers(0, 1, vertexBuffers, offsets);
        commandBuffer->cmdBindIndexBuffer(m_indexBuffer.getBuffer(), 0, VK_INDEX_TYPE_UINT16);
        commandBuffer->cmdBindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, &m_graphicsPipeline, 0, 1, &m_descriptorSets[currentFrame], 0, nullptr);

        commandBuffer->cmdDrawIndexed(static_cast<uint32_t>(g_indices.size()), 1, 0, 0, 0);
    }

    void RenderTask::createDescriptorSets(uint32_t maxFrameInFlight, VkDescriptorSetLayout descriptorSetLayout, const UniformBuffer* uniformBuffers, VkImageView textureImageView, VkSampler textureSampler)
    {
        std::vector<VkDescriptorSetLayout> layouts(maxFrameInFlight, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = maxFrameInFlight;
        allocInfo.pSetLayouts = layouts.data();

        m_descriptorSets.resize(maxFrameInFlight);
        m_descriptorPool.allocateDescriptorSets(&allocInfo, m_descriptorSets.data());

        for (size_t i = 0; i < maxFrameInFlight; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[i].getBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = textureImageView;
            imageInfo.sampler = textureSampler;

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = m_descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = m_descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(descriptorWrites.size()),
                                   descriptorWrites.data(), 0, nullptr);
        }
    }

    void RenderTask::createDescriptorPool(uint32_t maxFrameInFlight)
    {
        uint32_t descriptionCount = maxFrameInFlight;
        DescriptorPoolBuilder builder;
        builder.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptionCount);
        builder.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptionCount);
        builder.setMaxSet(descriptionCount);

        m_descriptorPool.create(&builder);
    }

    void RenderTask::release()
    {
        m_descriptorPool.release();

        m_indexBuffer.release();
        m_vertexBuffer.release();

        m_graphicsPipeline.release();
    }
} // narc_engine