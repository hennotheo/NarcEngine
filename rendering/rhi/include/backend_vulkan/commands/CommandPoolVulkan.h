//
// Created by theoh on 30/05/2025.
//

#pragma once

#include "commands/CommandPoolRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(CommandPool);

    class CommandPoolVulkan final : public CommandPoolRhi
    {
    public:
        NARC_IMPL_INITIALISABLE();
    };
} // narc_engine
