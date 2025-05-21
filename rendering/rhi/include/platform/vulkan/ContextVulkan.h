#pragma once

#include "rhi/ContextRhi.h"

namespace narc_engine
{
    class ContextVulkan : public ContextRhi
    {
    public:
        ContextVulkan() = default;
        ~ContextVulkan() override = default;

        void init() override;
        void shutdown() override;
    };
} // namespace narc_engine
