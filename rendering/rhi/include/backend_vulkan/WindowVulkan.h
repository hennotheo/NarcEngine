//
// Created by theoh on 27/05/2025.
//

#pragma once

#include "ContextRhi.h"
#include "WindowRhi.h"

namespace narc_engine
{
    class ContextVulkan;

    std::vector<const char*> getVulkanGLFWRequiredExtensions();

    class WindowVulkan final : public WindowRhi
    {
    public:
        explicit WindowVulkan(const ContextRhiPtr& ctx);
        ~WindowVulkan() override;

        NARC_IMPL_VK_PLATFORM_GETTERS(Window);

        NARC_GETTER(VkSurfaceKHR, getVkSurface, m_surface);

        NARC_IMPL_INITIALISABLE();

    private:
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    };
}
