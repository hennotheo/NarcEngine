//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class IFence;
    class SwapchainAcquireImageResult;

    using ImageIndex = uint32_t;

    class ISwapchain : public narc_core::IInitialisable
    {
    public:
        ~ISwapchain() override = default;

        NARC_PURE_VIRTUAL_GETTER(SurfaceExtent, getSwapChainExtent);
        NARC_PURE_VIRTUAL_QUERY(SwapchainAcquireImageResult, acquireNextImage, const ISemaphore* semaphore, const IFence* fence);
    };
}
