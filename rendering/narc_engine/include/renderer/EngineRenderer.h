#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "MultiFrameManager.h"

#include "models/Renderer.h"

#include "renderer/RenderTask.h"
#include "renderer/SwapChain.h"
#include "gui/UiRenderer.h"

#include "core/interfaces/ISurfaceObserver.h"
#include "renderer/render_graph/RenderGraph.h"

namespace narc_engine {


    class Material;
    class UniformBuffer;

    class EngineRenderer : public DeviceComponent
    {
        friend class EngineBinder;
        friend class Engine;

    public:
        EngineRenderer(const SwapChain* swapchain, MultiFrameManager* multiFrameManager);
        ~EngineRenderer();

        void prepareFrame(const FrameHandler* frameHandler);
        QUERY SignalSemaphores drawFrame(const FrameHandler* frameHandler, uint32_t imageIndex);

        void attachRenderer(const Renderer* renderer);

    private:
        std::map<uint32_t, RenderTask*> m_rendererTasks;

        MultiFrameManager* m_frameManager = nullptr;//TODO remove this dependency
        const SwapChain* m_swapchain = nullptr;//TODO remove this dependency

        // TEMP_CODE void createDescriptorSetLayout();
        UniformBufferObject updateUniformBuffer(UniformBuffer* buffer, RenderTask* rendererTask) const;
        void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex, const std::vector<VkDescriptorSet>& descriptorSets);
        RenderTask* createRenderTask(const Material* material);
    };
} // narc_engine
