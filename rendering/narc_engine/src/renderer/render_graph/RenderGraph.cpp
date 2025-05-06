#include "renderer/render_graph/RenderGraph.h"

namespace narc_engine
{
    void RenderGraph::addNode(RenderNode* node)
    {
        m_nodes.push_back(node);
    }

    void RenderGraph::removeNode(RenderNode* node)
    {
        auto iterator = std::remove(m_nodes.begin(), m_nodes.end(), node);
        if (iterator == m_nodes.end())
        {
            NARCLOG_DEBUG("RenderGraph::removeNode: Node not found in the graph.");
        }
        m_nodes.erase(iterator, m_nodes.end());
    }

    void RenderGraph::buildGraph()
    {
    }

    void RenderGraph::executeGraph(const FrameHandler* frameHandler, uint32_t imageIndex)
    {
        
    }
} // namespace narc_engine