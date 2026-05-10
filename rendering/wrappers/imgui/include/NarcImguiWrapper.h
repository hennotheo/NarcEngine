//
// Created by theo on 2/13/26.
//

#pragma once

#include "ImguiVulkanWrapper.h"

namespace narc_engine {
    class ImguiWrapper : narc_core::IInitialisable
    {
    public:
        explicit ImguiWrapper(std::unique_ptr<ImGuiBackend> backend);
        ~ImguiWrapper() override;

        NARC_IMPL_INITIALISABLE();

        void newFrame();
        void startWindow(const std::string& name);
        void text(const std::string& content);
        void spacing();
        void endWindow();
        void endFrame();
        void render(const ICommandBuffer* cmdBuffer);
        void drawDockspace();

    private:

        std::unique_ptr<ImGuiBackend> m_backend;
    };
}
