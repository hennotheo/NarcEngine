#include "core/EngineBinder.h"

#include "Engine.h"
#include "render_graph/RenderNode.h"

namespace narc_engine
{
    void EngineBinder::attachRenderer(const Renderer* renderer) const
    {
        static_cast<Engine*>(m_engine)->m_windows->addRenderer(renderer);
    }

    void EngineBinder::attachRenderNode(RenderNode* node) const
    {
        Engine::getInstance()->m_windows->m_renderGraph->addNode(node);
    }
}