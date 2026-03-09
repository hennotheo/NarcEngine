//
// Created by theohenno on 12/4/25.
//

#pragma once

namespace narc_engine {
    class IWindow;

    class ISurface : public narc_core::IInitialisable
    {
    public:
        ~ISurface() override = default;

        NARC_PURE_VIRTUAL_GETTER(const IWindow*, getMainSurface);
        NARC_PURE_VIRTUAL_GETTER(SurfaceExtent, getSurfaceExtent);
    };
}
