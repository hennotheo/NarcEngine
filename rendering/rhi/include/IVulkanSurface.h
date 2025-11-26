//
// Created by theohenno on 11/17/25.
//

#pragma once

namespace narc_engine {
    class VulkanDevice;
    
    class IVulkanSurface : public narc_core::IInitialisable
    {
    public:
        ~IVulkanSurface() override = default;

        NARC_PURE_VIRTUAL_GETTER(VkSurfaceKHR, getHandled);
        NARC_PURE_VIRTUAL_GETTER(bool, shouldClose); //TODO: Create IWindow later
        NARC_PURE_VIRTUAL_GETTER(VkExtent2D, getSurfaceExtent);
    };
} // narc_engine
