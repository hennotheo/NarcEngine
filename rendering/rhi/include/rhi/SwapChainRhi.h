//
// Created by theoh on 28/05/2025.
//

#pragma once

namespace narc_engine
{
    class WindowRhi;
    class DeviceRhi;

    NARC_DECL_RHI_PLATFORM_TYPES(SwapChain)

    class NARC_ENGINE_API SwapChainRhi : public narc_core::IInitialisable
    {
    public:
        SwapChainRhi();
        ~SwapChainRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(SwapChain)
    };

    using SwapChainRhiPtr = std::shared_ptr<SwapChainRhi>;
} // namespace narc_engine
