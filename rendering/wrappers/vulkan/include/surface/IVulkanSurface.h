//
// Created by theo on 2/19/26.
//

#pragma once

namespace narc_engine {
    class IVulkanSurface : public ISurface
    {
    public:
        NARC_PURE_VIRTUAL_GETTER(VkSurfaceKHR, getHandle);
    };

#ifdef NARC_ENGINE_PLATFORM_LINUX
    class VulkanLinuxSurface;
    using VulkanSurface = VulkanLinuxSurface;
#else
#error "Error"
#endif
}
