//
// Created by theoh on 5/23/2025.
//

#pragma once

#include "rhi/DebuggerRhi.h"

namespace narc_engine
{
    class DebuggerVulkan final : public DebuggerRhi
    {
    public:
        DebuggerVulkan();
        ~DebuggerVulkan() override;

        NARC_IMPL_INITIALISABLE();
    };

}
