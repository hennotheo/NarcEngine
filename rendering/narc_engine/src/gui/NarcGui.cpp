#include "gui/NarcGui.h"

#include "imgui.h"

namespace narc_gui
{
    void text(const char* text)
    {
        ImGui::Text(text);
    }
} // namespace narc_gui
