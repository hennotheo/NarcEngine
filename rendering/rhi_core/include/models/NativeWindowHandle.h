//
// Created by theo on 2/23/26.
//

#pragma once

namespace narc_engine {
    struct NativeWindowHandle
    {
#ifdef NARC_ENGINE_PLATFORM_LINUX

        xcb_connection_t* Connection;
        xcb_window_t Window;

#else

#error Handles not implemented.

#endif
    };
}
