//
// Created by theoh on 28/05/2025.
//
#include "SwapchainRhi.h"

namespace narc_engine
{
    SwapChainRhi::SwapChainRhi(FrameBufferRhiPtr& frameBuffer) :
        m_frameBuffer(std::move(frameBuffer))
    {
    }

    SwapChainRhi::~SwapChainRhi() = default;
} // namespace narc_engine
