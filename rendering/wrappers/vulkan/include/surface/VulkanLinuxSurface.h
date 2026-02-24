//
// Created by theo on 2/19/26.
//

#pragma once
#include "IVulkanSurface.h"

namespace narc_engine {
    class VulkanInstance;

    class VulkanLinuxSurface : public IVulkanSurface
    {
    public:
        explicit VulkanLinuxSurface(const VulkanInstance* instance, const IWindow* window);
        ~VulkanLinuxSurface() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(VkSurfaceKHR, getHandle, m_surface);
        NARC_OVERRIDE_GETTER(SurfaceExtent, getSurfaceExtent, m_extent)

        NARC_OVERRIDE_GETTER(const IWindow*, getMainSurface, m_window)

    private:
        const IWindow* m_window = nullptr;
        const VulkanInstance* m_instance = nullptr;

        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        SurfaceExtent m_extent{};
    };
}
