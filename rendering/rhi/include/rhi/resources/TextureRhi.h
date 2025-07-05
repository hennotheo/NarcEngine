//
// Created by theoh on 30/05/2025.
//

#pragma once

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(Texture);

    class NARC_ENGINE_API TextureRhi
    {

    };

    using TextureRhiPtr = std::unique_ptr<TextureRhi>;
} // narc_engine
