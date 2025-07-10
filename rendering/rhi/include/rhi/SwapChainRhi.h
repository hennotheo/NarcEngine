//
// Created by theoh on 28/05/2025.
//

#pragma once

#include "resources/ImageRhi.h"
#include "resources/ImageViewRhi.h"
#include "FrameBufferRhi.h"

namespace narc_engine
{
    using SwapChainExtends = glm::ivec2;

    NARC_DECL_RHI_PLATFORM_TYPES(SwapChain)

    class NARC_ENGINE_API SwapChainRhi : public narc_core::IInitialisable
    {
    public:
        SwapChainRhi();
        ~SwapChainRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(SwapChain)

        NARC_PURE_VIRTUAL_GETTER(SwapChainExtends, getExtends);

    protected:
        std::vector<FrameBufferRhi> m_framebuffers{};
        std::vector<std::shared_ptr<ImageRhi>> m_images{};//TODO: Change this to a more specific type later
        std::vector<std::shared_ptr<ImageViewRhi>> m_imageViews{};//TODO: Change this to a more specific type later

        void createImageViews();
    };

    using SwapChainRhiPtr = std::shared_ptr<SwapChainRhi>;
} // namespace narc_engine
