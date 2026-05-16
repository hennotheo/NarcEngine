//
// Created by theo on 5/15/26.
//


#pragma once

#include "IVulkanSurface.h"

namespace narc_engine {
    class VulkanInstance;

    class VulkanHeadlessSurface : public IVulkanSurface
    {
    public:
        explicit VulkanHeadlessSurface(const VulkanInstance* instance);
        ~VulkanHeadlessSurface() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(VkSurfaceKHR, getHandle, m_surface);
        NARC_OVERRIDE_GETTER(SurfaceExtent, getSurfaceExtent, m_extent);

        NARC_OVERRIDE_GETTER(const IWindow*, getMainSurface, nullptr);

    private:
        const VulkanInstance* m_instance = nullptr;

        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        SurfaceExtent m_extent{};
    };
}
