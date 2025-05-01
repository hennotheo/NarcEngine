#pragma once

#include "core/interfaces/ISurfaceProvider.h"

namespace narc_engine {
    class ISurfaceObserver
    {
        friend class ISurfaceProvider;
    protected:
        inline virtual void onSurfaceFramebufferResized(int width, int height) { };
    };
} // namespace narc_engine