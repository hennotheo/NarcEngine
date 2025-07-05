//
// Created by theoh on 30/05/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES (CommandQueue);

    class NARC_ENGINE_API CommandQueueRhi : public narc_core::IInitialisable
    {

    };

    using CommandQueueRhiPtr = std::shared_ptr<CommandQueueRhi>;
} // narc_engine
