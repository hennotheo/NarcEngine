//
// Created by theoh on 25/05/2025.
//

#pragma once

#include "ContextRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(Device)

    class ContextRhi;

    class NARC_ENGINE_API DeviceRhi : public narc_core::IInitialisable
    {
    public:
        explicit DeviceRhi(const ContextRhi& ctx);
        ~DeviceRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(Device);
    };

    NARC_DECL_RHI_CREATION(DeviceRhi, const ContextRhi& ctx);
}
