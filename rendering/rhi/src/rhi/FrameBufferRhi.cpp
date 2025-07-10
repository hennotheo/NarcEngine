//
// Created by theoh on 7/7/2025.
//

#include "FrameBufferRhi.h"

#include "resources/ImageViewRhi.h"

namespace narc_engine
{
    FrameBufferRhi::FrameBufferRhi() = default;

    FrameBufferRhi::~FrameBufferRhi() = default;

    void FrameBufferRhi::attachImageView(const ImageViewRhi* imageView)
    {
        m_imageViews.push_back(imageView);
    }
} // narc_engine
