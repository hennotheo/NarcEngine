//
// Created by theoh on 28/05/2025.
//

#pragma once

#include "SwapChainRhi.h"

namespace narc_engine
{
    class SwapChainVulkan final : public SwapChainRhi
    {
    public:
        explicit SwapChainVulkan(const WindowRhi& window, const DeviceRhi& device);
        ~SwapChainVulkan() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkSwapchainKHR, getVkSwapChain, m_swapChain);

        NARC_IMPL_RHI_PLATFORM_GETTER(SwapChain, Vulkan);

    private:
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

        const WindowRhi& m_window;
        const DeviceRhi& m_device;
    };
} // narc_engine
