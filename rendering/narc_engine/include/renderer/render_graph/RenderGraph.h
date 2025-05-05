#pragma once

namespace narc_engine
{
    class RenderNode;
    class FrameHandler;

    // TODO, currently in early allocation, this should use a lazy allocation system
    class RenderGraph
    {
        RenderGraph() = default;
        ~RenderGraph() = default;

        void addNode(RenderNode* node);
        void removeNode(RenderNode* node);

        void buildGraph();
        void executeGraph(const FrameHandler* frameHandler, uint32_t imageIndex);

    private:
        std::vector<RenderNode*> m_nodes;
    };
} // namespace narc_engine
