//
// Created by theohenno on 11/17/25.
//

#pragma once

namespace narc_engine {
    class VulkanInstance;

    class IVulkanSurface : public narc_core::IInitialisable
    {
    public:
        ~IVulkanSurface() override = default;

        NARC_PURE_VIRTUAL_GETTER(VkSurfaceKHR, getHandled);
        NARC_PURE_VIRTUAL_GETTER(bool, shouldClose); //TODO: Create IWindow later
    };

    class IVulkanSurfacesManager : public narc_core::IInitialisable
    {
    public:
        ~IVulkanSurfacesManager() override = default;

        NARC_PURE_VIRTUAL_GETTER(const IVulkanSurface*, getMainSurface);

        virtual void updateSurfaces() = 0;
        
        virtual void pushSurface(std::unique_ptr<IVulkanSurface> surface) = 0;
    };
} // narc_engine
