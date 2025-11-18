//
// Created by theohenno on 11/17/25.
//

#pragma once

namespace narc_engine {
    class VulkanInstance;

    class IIVulkanSurface : public narc_core::IInitialisable
    {
    protected:
        ~IIVulkanSurface() override = default;

    public:
        NARC_PURE_VIRTUAL_GETTER(VkSurfaceKHR, getSurface);
        NARC_PURE_VIRTUAL_GETTER(bool, shouldClose);
    };
} // narc_engine
