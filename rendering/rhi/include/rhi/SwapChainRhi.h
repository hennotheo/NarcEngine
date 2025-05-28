//
// Created by theoh on 28/05/2025.
//

#pragma once

#include "DeviceRhi.h"
#include "WindowRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(SwapChain)

    class NARC_ENGINE_API SwapChainRhi : public narc_core::IInitialisable
    {
    public:
        explicit SwapChainRhi(const WindowRhi* window, const DeviceRhi* device);
        ~SwapChainRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(SwapChain)

    protected:
        const WindowRhi* m_window;
        const DeviceRhi* m_device;
    };

    NARC_DECL_RHI_CREATION(SwapChainRhi, const WindowRhi* window, const DeviceRhi* device);
}
