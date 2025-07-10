//
// Created by theoh on 7/10/2025.
//

#pragma once

#include "resources/ImageViewRhi.h"

namespace narc_engine
{
    class ImageViewVulkan final : public ImageViewRhi
    {
    public:
        ImageViewVulkan();
        ~ImageViewVulkan() override;

        NARC_IMPL_INITIALISABLE();
    };
} // narc_engine
