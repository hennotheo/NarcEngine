//
// Created by theoh on 7/9/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES (ImageView);

    class NARC_ENGINE_API ImageViewRhi : public narc_core::IInitialisable
    {
    public:
        ImageViewRhi() = default;
        ~ImageViewRhi() override = default;

        NARC_DECL_RHI_PLATFORM_GETTERS (ImageView);
    };

    using ImageViewRhiPtr = std::shared_ptr<ImageViewRhi>;
} // narc_engine
