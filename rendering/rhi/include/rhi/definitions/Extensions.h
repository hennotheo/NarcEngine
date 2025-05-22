//
// Created by theoh on 5/22/2025.
//

#pragma once

namespace narc_engine
{
    enum class RhiExtensions
    {
        ///@brief All extensions needed to run the engine
        Core,
        DebugUtils,
        Surface,
        ExtendedDevicesProperties,
        ExtendedSurfaceCapabilities
    };

    constexpr RhiExtensions CoreExtension[1] = {RhiExtensions::Core};//To simplify the code
} // namespace narc_engine
