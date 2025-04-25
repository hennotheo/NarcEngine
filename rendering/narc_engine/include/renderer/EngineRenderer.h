#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "MultiFrameManager.h"

#include "models/Renderer.h"

#include "renderer/RenderTask.h"
#include "renderer/SwapChain.h"
#include "gui/UiRenderer.h"

namespace narc_engine {
    class Material;
    class UniformBuffer;

    class EngineRenderer : public DeviceComponent
    {
        friend class EngineBinder;
        friend class Engine;

    public:
        EngineRenderer(const EngineInstance* instance);
        ~EngineRenderer();

        void drawFrame();
        void updateUniformBuffer(UniformBuffer* buffer, RenderTask* rendererTask) const;

        void attachRenderer(const Renderer* renderer);

    private:
        SwapChain m_swapChain;
        std::unique_ptr<MultiFrameManager> m_frameManager;
        std::unique_ptr<UiRenderer> m_uiRenderer;
        std::map<uint32_t, RenderTask*> m_rendererTasks;

        VkDescriptorSetLayout m_descriptorSetLayout;

        void createDescriptorSetLayout();
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex, const std::vector<VkDescriptorSet>& descriptorSets);
        RenderTask* createRenderTask(const Material* material);
    };
} // narc_engine
