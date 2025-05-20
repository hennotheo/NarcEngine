#pragma once

#include <vulkan/vulkan.h>

#include "models/UniformBufferObject.h"

namespace narc_engine
{
    typedef const std::vector<VkSemaphore> SignalSemaphores;

    class RenderNode;
    class Renderer;
    class FrameHandler;
    class UniformBuffer;
    class SwapChain;
    class RenderContext;
    class CommandBuffer;

    // TODO, currently in early allocation, this should use a lazy allocation system
    class RenderGraph
    {
    public:
        RenderGraph(const SwapChain* swapchain) : m_swapchain(swapchain) {};
        ~RenderGraph();

        void addNode(RenderNode* node);
        void removeNode(RenderNode* node);

        std::vector<const Renderer*> m_renderers;//TODO remove this, this is a temporary solution

        void buildGraph();
        SignalSemaphores executeGraph(const FrameHandler* frameHandler, uint32_t imageIndex);

    private:
        std::vector<RenderNode*> m_nodes;

        const SwapChain* m_swapchain = nullptr;//TODO remove this dependency

        UniformBufferObject updateUniformBuffer(UniformBuffer* buffer, std::vector<const Renderer*>& renderers) const;
        void allocateResources(const FrameHandler* frameHandler);
        void recordCommandBuffer(CommandBuffer* commandBuffer, const FrameHandler* frameHandler, const uint32_t imageIndex);
        void fillRenderContext(RenderContext* ctx, const FrameHandler* frameHandler);
        bool checkIfInputsAreAvailable(const RenderNode* node, const std::set<ResourceId>& availableResources) const;
    };
} // namespace narc_engine
