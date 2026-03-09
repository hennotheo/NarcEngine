//
// Created by theo on 2/26/26.
//

#pragma once

namespace narc_engine {
    class ICommandBufferPool : public narc_core::IInitialisable
    {
    public:
        NARC_PURE_VIRTUAL_QUERY(RhiQuery<std::unique_ptr<ICommandBuffer>>, allocateCommandBuffer);
        NARC_PURE_VIRTUAL_QUERY(RhiQuery<std::unique_ptr<ICommandBuffer>>, allocateOneTimeBuffer);
        virtual void destroyOneTimeBuffer(const ICommandBuffer* cmd) = 0;
    };
}