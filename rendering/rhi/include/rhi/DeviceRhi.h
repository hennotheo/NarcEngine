//
// Created by theoh on 25/05/2025.
//

#pragma once

#include "SwapChainRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(Device)

    class ContextRhi;
    class WindowRhi;

    class NARC_ENGINE_API DeviceRhi : public narc_core::IInitialisable
    {
    public:
        explicit DeviceRhi(const ContextRhi* ctx);
        ~DeviceRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(Device);

    PROTECTED_TESTABLE:
        const ContextRhi* m_context;

        SwapChainRhiPtr m_swapChain;
    };

    NARC_DECL_RHI_CREATION(DeviceRhi, const ContextRhi* ctx);
}
