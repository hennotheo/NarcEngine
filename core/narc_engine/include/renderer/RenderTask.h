#pragma once
#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "SwapChain.h"
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
        void createDescriptorSets(uint32_t maxFrameInFlight, VkDescriptorSetLayout descriptorSetLayout,
                                  const UniformBuffer* uniformBuffers, VkImageView
                                  textureImageView, VkSampler textureSampler, const DescriptorPool* descriptorPool);

        void release();

    private:
        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        std::unique_ptr<GraphicsBuffer<Vertex>> m_vertexBuffer;
        std::unique_ptr<GraphicsBuffer<uint16_t>> m_indexBuffer;

        std::vector<VkDescriptorSet> m_descriptorSets;

        VkDevice m_device;

        void createGraphicsPipeline(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);
    };
} // narc_engine
