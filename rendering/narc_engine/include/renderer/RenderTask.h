#pragma once

#include "CommandBuffer.h"
#include "DescriptorPool.h"
#include "GraphicsPipeline.h"
#include "SwapChain.h"
#include "buffers/UniformBuffer.h"
#include "models/Material.h"

namespace narc_engine
{
    class Renderer;

    class RenderTask : public DeviceComponent
    {
    public:
        RenderTask(const SwapChain* swapChain, const Material* material);
        ~RenderTask();

        GETTER VkDeviceSize getUniformBufferSize() const { return sizeof(UniformBufferObject); }

        void recordTask(const CommandBuffer* commandBuffer, const VkDescriptorSet* m_descriptorSet) const;
        void updateDescriptorSet(VkDescriptorSet descriptorSets, const VkDescriptorBufferInfo* uniformBuffersInfo) const;

        void bindRenderer(const Renderer* renderer) { m_renderers.push_back(renderer); }
        void unbindRenderer(const Renderer* renderer) { std::erase(m_renderers, renderer); }

        DEPRECATED GETTER std::vector<const Renderer*>* getRenderers() { return &m_renderers; }

    private:
        std::unique_ptr<GraphicsPipeline> m_pipeline;
        std::vector<const Renderer*> m_renderers;

        const Material* m_material;
    };
} // narc_engine
