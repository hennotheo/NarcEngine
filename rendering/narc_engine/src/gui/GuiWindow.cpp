#include "gui/GuiWindow.h"

#include "imgui.h"

namespace narc_engine
{
    void GuiWindow::onRender() const
    {
        ImGui::Begin(m_name.c_str());
        renderContent();
        ImGui::End();
    }
} // namespace narc_engine

