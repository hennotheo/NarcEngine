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
        RenderTask(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);
        ~RenderTask();

        void recordTask(const CommandBuffer* commandBuffer, const VkDescriptorSet* m_descriptorSet) const;
        void updateDescriptorSets(uint32_t currentFrameID,
                                  const std::vector<VkDescriptorSet>& descriptorSets, const UniformBuffer* uniformBuffers, VkImageView
                                  textureImageView, VkSampler textureSampler) const;

        void bindRenderer(const Renderer* renderer) { m_renderers.push_back(renderer); }
        void unbindRenderer(const Renderer* renderer) { std::erase(m_renderers, renderer); }

    private:
        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        std::vector<const Renderer*> m_renderers;
        // std::vector<VkDescriptorSet> m_descriptorSets;

        VkDevice m_device;

        void createGraphicsPipeline(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);
    };
} // narc_engine
