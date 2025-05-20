#pragma once

#include "renderer/GraphicsPipeline.h"

namespace narc_engine
{
    class CommandBuffer;
    class RenderContext;

    class NARC_ENGINE_API RenderNode
    {
    public:
        RenderNode(const RenderPass* renderPass, const Shader* shader);
        virtual ~RenderNode() = default;

        GETTER uint32_t getPriority() const { return 0; }//TODO change to priority system

        GETTER const std::vector<ResourceId>& getOutputs()const { return m_outputs; }
        GETTER const std::vector<ResourceId>& getInputs() const { return m_inputs; }

        TEMP_CODE void record(const CommandBuffer* cmd, const RenderContext* ctx);//TODO Add pure virtual function

    private:
        std::unique_ptr<GraphicsPipeline> m_pipeline;

        std::vector<ResourceId> m_inputs;
        std::vector<ResourceId> m_outputs;
    };
} // namespace narc_engine
