#pragma once

namespace narc_engine {
    class ISurfaceProvider;

    class ISurfaceObserver
    {
        friend class ISurfaceProvider;
    protected:
        inline virtual void onSurfaceFramebufferResized(int width, int height) { };
    };
} // namespace narc_engine