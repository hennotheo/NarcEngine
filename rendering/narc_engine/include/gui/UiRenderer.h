#pragma once

namespace narc_engine
{
    class EngineInstance;
    class MultiFrameManager;
    class SwapChain;
    class CommandBuffer;
    class ISurfaceProvider;
    class IGuiHandle;

    class UiRenderer
    {
    public:
        UiRenderer(const EngineInstance* instance, const MultiFrameManager* frameManager, const SwapChain* swapChain, const ISurfaceProvider* surface);
        ~UiRenderer();

        void addGuiComponent(const IGuiHandle* component) { m_guiComponents.push_back(component); }

        void beginFrame();
        void render(const CommandBuffer* commandBuffer);

    private: 
        std::vector<const IGuiHandle*> m_guiComponents;
    };
}