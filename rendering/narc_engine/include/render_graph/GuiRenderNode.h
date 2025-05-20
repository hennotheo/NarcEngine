#pragma once

#include "RenderNode.h"

namespace narc_engine
{
    class CommandBuffer;
    class RenderContext;
    class MultiFrameManager;
    class Window;
    class SwapChain;

    class GuiRenderNode : public RenderNode
    {
    public:
        explicit GuiRenderNode(const RenderPass* renderPass, const SwapChain* swapchain, const MultiFrameManager* frameManager, const Window* window);
        ~GuiRenderNode() override;

        void record(const CommandBuffer* cmd, const RenderContext* ctx) override;
    };
} // namespace narc_engine
