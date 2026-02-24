//
// Created by theo on 2/19/26.
//

#pragma once

namespace narc_engine {
    class ISurface : public narc_core::IInitialisable
    {
    public:

        NARC_PURE_VIRTUAL_GETTER(VkSurfaceKHR, getHandle);
    };
}
