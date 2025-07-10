//
// Created by theoh on 7/9/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(Image);

    class NARC_ENGINE_API ImageRhi : public narc_core::IInitialisable
    {
    public:
        ImageRhi() = default;
        ~ImageRhi() override = default;

        NARC_DECL_RHI_PLATFORM_GETTERS(Image);
    };

    using ImageRhiPtr = std::shared_ptr<ImageRhi>;
} // narc_engine
