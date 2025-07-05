//
// Created by theoh on 30/05/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES (CommandBuffer);

    class NARC_ENGINE_API CommandBufferRhi : public narc_core::IInitialisable
    {

    };

    using CommandBufferRhiPtr = std::unique_ptr<CommandBufferRhi>;
} // narc_engine
