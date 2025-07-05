#pragma once

#include "renderer/GraphicsPipeline.h"

namespace narc_engine
{
    class CommandBuffer;
    class RenderContext;

    class RenderNode
    {
    public:
        RenderNode() = default;
        explicit RenderNode(const RenderPass* renderPass, const Shader* shader);
        virtual ~RenderNode() = default;

        uint32_t getOrder() const { return m_order; }

        const std::vector<ResourceId>& getOutputs()const { return m_outputs; }
        const std::vector<ResourceId>& getInputs() const { return m_inputs; }

        TEMP_CODE virtual void record(const CommandBuffer* cmd, const RenderContext* ctx);//TODO Add pure virtual function

    protected:
        uint32_t m_order = 0;

    private:
        std::unique_ptr<GraphicsPipeline> m_pipeline;

        std::vector<ResourceId> m_inputs;
        std::vector<ResourceId> m_outputs;
    };
} // namespace narc_engine
