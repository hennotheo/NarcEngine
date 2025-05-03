#include "core/EngineBinder.h"

#include "Engine.h"

namespace narc_engine
{
    void EngineBinder::attachRenderer(const Renderer* renderer) const
    {
        static_cast<Engine*>(m_engine)->m_windows->m_renderer->attachRenderer(renderer);
    }

    void EngineBinder::attachGuiComponent(const IGuiHandle* component) const
    {
        static_cast<Engine*>(m_engine)->m_windows->m_renderer->m_uiRenderer->addGuiComponent(component);
    }
}