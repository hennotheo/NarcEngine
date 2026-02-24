//
// Created by theo on 2/19/26.
//

#pragma once
#include "ISurface.h"

namespace narc_engine {
    class VulkanInstance;

    class VulkanLinuxSurface : public ISurface
    {
    public:
        explicit VulkanLinuxSurface(const VulkanInstance* instance, const IWindow* window);
        ~VulkanLinuxSurface() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(VkSurfaceKHR, getHandle, m_surface);

    private:
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        const IWindow* m_window = nullptr;
        const VulkanInstance* m_instance;
    };
}
