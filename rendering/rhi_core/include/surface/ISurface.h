//
// Created by theohenno on 11/17/25.
//

#pragma once

namespace narc_engine {
    class ISurface : public narc_core::IHandler, public narc_core::IInitialisable
    {
    public:
        ~ISurface() override = default;

        NARC_PURE_VIRTUAL_GETTER(bool, shouldClose); //TODO: Create IWindow later
        NARC_PURE_VIRTUAL_GETTER(SurfaceExtend, getSurfaceExtent);
        NARC_PURE_VIRTUAL_GETTER(void*, getNativeHandle);
    };
} // narc_engine
