#pragma once
#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "SwapChain.h"
#include "buffers/UniformBuffer.h"
#include "data/Mesh.h"
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
        void bindMesh(const Mesh* mesh) { m_meshes.push_back(mesh); }
        void unbindMesh(const Mesh* mesh) { m_meshes.erase(std::remove(m_meshes.begin(), m_meshes.end(), mesh), m_meshes.end()); }

        void release();

    private:
        VkPipeline m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        std::vector<const Mesh*> m_meshes;
        std::vector<VkDescriptorSet> m_descriptorSets;

        VkDevice m_device;

        void createGraphicsPipeline(const SwapChain* swapChain, const VkDescriptorSetLayout* m_descriptorSetLayout);
    };
} // narc_engine
