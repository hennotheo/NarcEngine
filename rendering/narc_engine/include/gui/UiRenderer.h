#pragma once

namespace narc_engine
{
    class EngineInstance;
    class MultiFrameManager;
    class SwapChain;

    class UiRenderer
    {
    public:
        UiRenderer(const EngineInstance* instance, const MultiFrameManager* frameManager, const SwapChain* swapChain);
        ~UiRenderer();
    };
}