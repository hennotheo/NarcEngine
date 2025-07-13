//
// Created by theoh on 27/05/2025.
//

#pragma once

#include "WindowRhi.h"

#include "ContextRhi.h"
#include "SwapChainVulkan.h"

namespace narc_engine
{
    std::vector<const char*> getVulkanGLFWRequiredExtensions();

    class WindowVulkan final : public WindowRhi
    {
    public:
        BOOST_DI_INJECT(WindowVulkan, const ContextRhiPtr& ctx, const DeviceRhiPtr& device);
        ~WindowVulkan() override;

        NARC_IMPL_VK_PLATFORM_GETTERS(Window);

        NARC_GETTER(VkSurfaceKHR, getVkSurface, m_surface);

        NARC_IMPL_INITIALISABLE();

    private:
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;

        const SwapChainRhiPtr m_swapChain;
    };
}
