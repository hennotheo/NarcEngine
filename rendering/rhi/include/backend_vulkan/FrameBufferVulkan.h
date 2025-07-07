//
// Created by theoh on 7/7/2025.
//

#pragma once

#include "FrameBufferRhi.h"

namespace narc_engine
{
    class FrameBufferVulkan final : public FrameBufferRhi
    {
        using super = FrameBufferRhi;

    public:
        FrameBufferVulkan();
        ~FrameBufferVulkan() override;

        NARC_IMPL_INITIALISABLE();

        NARC_IMPL_RHI_PLATFORM_GETTER(FrameBuffer, Vulkan);
    };
} // narc_engine
