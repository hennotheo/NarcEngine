#pragma once

namespace narc_engine
{
    class CommandBuffer;

    class RenderNode
    {
    public:
        RenderNode() = default;
        virtual ~RenderNode() = default;

        TEMP_CODE virtual void record(const CommandBuffer* cmd);//TODO change to pure virtual function
    };
} // namespace narc_engine
