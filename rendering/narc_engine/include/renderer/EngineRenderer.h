#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "MultiFrameManager.h"

#include "models/Renderer.h"

#include "renderer/RenderTask.h"
#include "renderer/SwapChain.h"
#include "gui/UiRenderer.h"

#include "core/interfaces/ISurfaceObserver.h"

namespace narc_engine {
    class Material;
    class UniformBuffer;

    class EngineRenderer : public DeviceComponent, public ISurfaceObserver
    {
        friend class EngineBinder;
        friend class Engine;

    public:
        EngineRenderer(const EngineInstance* instance, ISurfaceProvider* surfaceProvider, MultiFrameManager* multiFrameManager);
        ~EngineRenderer();

        void drawFrame(const FrameHandler* frameHandler);
        void updateUniformBuffer(UniformBuffer* buffer, RenderTask* rendererTask) const;

        void attachRenderer(const Renderer* renderer);

    protected:
        void onSurfaceFramebufferResized(int width, int height) override;

    private:
        SwapChain m_swapChain;
        std::unique_ptr<UiRenderer> m_uiRenderer;
        std::map<uint32_t, RenderTask*> m_rendererTasks;

        bool m_framebufferResized = false;

        VkDescriptorSetLayout m_descriptorSetLayout;

        MultiFrameManager* m_frameManager = nullptr;

        void createDescriptorSetLayout();
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex, const std::vector<VkDescriptorSet>& descriptorSets);
        RenderTask* createRenderTask(const Material* material);
    };
} // narc_engine
