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

    NARC_DECL_RHI_CREATION(CommandQueueRhi);
} // narc_engine
