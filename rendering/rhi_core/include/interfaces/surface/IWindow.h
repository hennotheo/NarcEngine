//
// Created by theohenno on 11/17/25.
//

#pragma once

#include "models/NativeWindowHandle.h"

namespace narc_engine {
    class IWindow : public narc_core::IInitialisable
    {
    public:
        ~IWindow() override = default;

        NARC_PURE_VIRTUAL_SETTER(std::string, setTitle);

        NARC_PURE_VIRTUAL_QUERY(bool, shouldClose);
        NARC_PURE_VIRTUAL_QUERY(SurfaceExtent, getSurfaceExtent);
        NARC_PURE_VIRTUAL_GETTER(NativeWindowHandle, getNativeHandle);

        NARC_PURE_VIRTUAL_GETTER(std::string_view, getTitle);
    };
} // narc_engine
