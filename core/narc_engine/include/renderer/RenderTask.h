#pragma once
#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "buffers/GraphicsBuffer.h"
#include "buffers/UniformBuffer.h"
#include "data/Vertex.h"

namespace narc_engine
{
    class RenderTask
    {
    public:
        void create(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);

        void recordTask(const CommandBuffer* commandBuffer, uint32_t currentFrame);
        void createDescriptorSets(uint32_t maxFrameInFlight, VkDescriptorSetLayout descriptorSetLayout, const UniformBuffer* uniformBuffers, VkImageView
                                  textureImageView, VkSampler textureSampler);
        void createDescriptorPool(uint32_t maxFrameInFlight);

        void release();
    private:
        Pipeline m_graphicsPipeline;

        GraphicsBuffer<Vertex> m_vertexBuffer;
        GraphicsBuffer<uint16_t> m_indexBuffer;

        DescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;

        VkDevice m_device;
    };
} // narc_engine
