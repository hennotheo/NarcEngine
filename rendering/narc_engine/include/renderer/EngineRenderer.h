#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "MultiFrameManager.h"
#include "models/Renderer.h"
#include "renderer/RenderTask.h"
#include "renderer/SwapChain.h"

namespace narc_engine {
    class Material;
    class UniformBuffer;

    class EngineRenderer
    {
        friend class EngineBinder;
        friend class Engine;

    public:
        EngineRenderer();
        ~EngineRenderer();

        void drawFrame();
        void updateUniformBuffer(UniformBuffer* buffer) const;

        void attachRenderer(const Renderer* renderer);

    private:
        SwapChain m_swapChain;
        std::unique_ptr<MultiFrameManager> m_frameManager;
        std::map<uint32_t, RenderTask*> m_rendererTasks;

        VkDescriptorSetLayout m_descriptorSetLayout;

        const DeviceHandler* m_device = nullptr;

        void createDescriptorSetLayout();
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex, const std::vector<VkDescriptorSet>& descriptorSets);
        RenderTask* createRenderTask(const Material* material);
    };
} // narc_engine
