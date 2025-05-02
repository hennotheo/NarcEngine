#include "gui/GuiWindow.h"

#include "imgui.h"

namespace narc_gui
{
    void GuiWindow::onRender() const
    {
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
        ImGui::Begin(m_name.c_str(), nullptr, windowFlags);

        ImGui::SetWindowPos(ImVec2(m_position.x, m_position.y), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(m_size.x, m_size.y), ImGuiCond_Always);

        ImGui::PushTextWrapPos(ImGui::GetWindowWidth() - 20);
        ImGui::TextWrapped(m_name.c_str());
        
        renderContent();
        
        ImGui::PopTextWrapPos();
        ImGui::End();
    }
} // namespace narc_gui

