#pragma once

#include "renderer/GraphicsPipeline.h"

namespace narc_engine
{
    class CommandBuffer;
    class RenderContext;

    class RenderNode
    {
    public:
        RenderNode(const RenderPass* renderPass, const Shader* vertShader, const Shader* fragShader);
        virtual ~RenderNode() = default;

        GETTER uint32_t getPriority() const { return 0; }//TODO change to priority system

        TEMP_CODE virtual void record(const CommandBuffer* cmd, const RenderContext* ctx);//TODO change to pure virtual function
    
    private:
        std::unique_ptr<GraphicsPipeline> m_pipeline;
    };
} // namespace narc_engine
