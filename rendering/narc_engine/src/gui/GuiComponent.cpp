#include "gui/GuiComponent.h"

namespace narc_gui
{
    void GuiComponent::show()
    {
        m_visible = true;
    }

    void GuiComponent::hide()
    {
        m_visible = false;
    }

    void GuiComponent::render() const
    {
        if (!m_visible)
            return;

        onRender();
    }
} // namespace narc_gui
