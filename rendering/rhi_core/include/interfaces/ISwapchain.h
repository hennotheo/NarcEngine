//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class ISwapchain : public narc_core::IInitialisable
    {
    public:
        ~ISwapchain() override = default;

        NARC_PURE_VIRTUAL_GETTER(SurfaceExtent, getSwapChainExtent);
    };
}