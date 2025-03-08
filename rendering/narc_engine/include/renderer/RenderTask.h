#pragma once

#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "SwapChain.h"
#include "buffers/UniformBuffer.h"

namespace narc_engine
{
    class Renderer;

    class RenderTask
    {
    public:
        inline bool isCreated() const { return m_isCreated; }

        void create(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);

        void recordTask(const CommandBuffer* commandBuffer, uint32_t currentFrame);
        void createDescriptorSets(uint32_t maxFrameInFlight, VkDescriptorSetLayout descriptorSetLayout,
                                  const UniformBuffer* uniformBuffers, VkImageView
                                  textureImageView, VkSampler textureSampler, const DescriptorPool* descriptorPool);
        void bindRenderer(const Renderer* renderer) { m_renderers.push_back(renderer); }

        void unbindRenderer(const Renderer* renderer)
        {
            std::erase(m_renderers, renderer);
        }

        void release();

    private:
        bool m_isCreated = false;

        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        std::vector<const Renderer*> m_renderers;
        std::vector<VkDescriptorSet> m_descriptorSets;

        VkDevice m_device;

        void createGraphicsPipeline(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);
    };
} // narc_engine
