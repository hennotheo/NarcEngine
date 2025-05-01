#pragma once

namespace narc_engine
{
    class EngineInstance;
    class MultiFrameManager;
    class SwapChain;
    class CommandBuffer;
    class ISurfaceProvider;

    class UiRenderer
    {
    public:
        UiRenderer(const EngineInstance* instance, const MultiFrameManager* frameManager, const SwapChain* swapChain, const ISurfaceProvider* surface);
        ~UiRenderer();

        void beginFrame();
        void render(const CommandBuffer* commandBuffer);
    };
}