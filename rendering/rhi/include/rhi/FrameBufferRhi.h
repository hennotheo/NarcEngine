//
// Created by theoh on 7/7/2025.
//

#pragma once

#include "resources/ImageViewRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(FrameBuffer);

    class NARC_ENGINE_API FrameBufferRhi : public narc_core::IInitialisable
    {
    public:
        FrameBufferRhi();
        ~FrameBufferRhi() override;

        void attachImageView(const ImageViewRhi* imageView);

        NARC_DECL_RHI_PLATFORM_GETTERS(FrameBuffer);

    protected:
        NARC_GETTER(const std::vector<const ImageViewRhi*>&, getImageViews, m_imageViews);

    private:
        std::vector<const ImageViewRhi*> m_imageViews{};
    };

    using FrameBufferRhiPtr = std::unique_ptr<FrameBufferRhi>;
}
