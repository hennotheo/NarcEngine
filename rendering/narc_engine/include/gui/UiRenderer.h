#pragma once

namespace narc_engine
{
    class EngineInstance;
    class MultiFrameManager;
    class SwapChain;
    class CommandBuffer;
    class ISurfaceProvider;
    class IGuiHandle;
}

namespace narc_gui
{
    class UiRenderer
    {
    public:
        UiRenderer(const narc_engine::EngineInstance* instance, const narc_engine::MultiFrameManager* frameManager, const narc_engine::SwapChain* swapChain, const narc_engine::ISurfaceProvider* surface);
        ~UiRenderer();

        void addGuiComponent(const narc_engine::IGuiHandle* component) { m_guiComponents.push_back(component); }

        void beginFrame();
        void render(const narc_engine::CommandBuffer* commandBuffer);

    private: 
        std::vector<const narc_engine::IGuiHandle*> m_guiComponents;
    };
}