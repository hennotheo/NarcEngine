#include "core/EngineBinder.h"

#include "Engine.h"

namespace narc_engine
{
    void EngineBinder::attachRenderer(const Renderer* renderer) const
    {
        static_cast<Engine*>(m_engine)->m_renderer->attachRenderer(renderer);
    }
}
