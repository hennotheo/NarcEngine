//
// Created by theoh on 28/05/2025.
//

#pragma once

#include "FrameBufferRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(SwapChain)

    class NARC_ENGINE_API SwapChainRhi : public narc_core::IInitialisable
    {
    public:
        explicit SwapChainRhi(FrameBufferRhiPtr& frameBuffer);
        ~SwapChainRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(SwapChain)

    protected:
        const FrameBufferRhiPtr m_frameBuffer;
    };

    using SwapChainRhiPtr = std::shared_ptr<SwapChainRhi>;
} // namespace narc_engine
