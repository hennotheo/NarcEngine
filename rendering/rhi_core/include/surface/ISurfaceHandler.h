//
// Created by theohenno on 12/4/25.
//

#pragma once

namespace narc_engine {
    class ISurface;

    class ISurfacesHandler : public narc_core::IService
    {
    public:
        ~ISurfacesHandler() override = default;

        NARC_PURE_VIRTUAL_GETTER(const ISurface*, getMainSurface);
    };
}
