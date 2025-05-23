//
// Created by theoh on 5/22/2025.
//

#pragma once

namespace narc_engine
{
    enum class RhiExtension
    {
        ///@brief All extensions needed to run the engine
        Core,
        DebugUtils,
        Surface,
        ExtendedDevicesProperties,
        ExtendedSurfaceCapabilities
    };

    constexpr RhiExtension CoreExtension[1] = {RhiExtension::Core};//To simplify the code
} // namespace narc_engine
