//
// Created by theoh on 30/05/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(CommandPool);

    class NARC_ENGINE_API CommandPoolRhi : public narc_core::IInitialisable
    {
    public:
        CommandPoolRhi() = default;
        ~CommandPoolRhi() override = default;

        NARC_DECL_RHI_PLATFORM_GETTERS(CommandPool);
    };

    using CommandPoolRhiPtr = std::shared_ptr<CommandPoolRhi>;
} // narc_engine
