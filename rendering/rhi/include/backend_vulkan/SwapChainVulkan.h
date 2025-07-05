//
// Created by theoh on 28/05/2025.
//

#pragma once

#include "DeviceRhi.h"
#include "SwapChainRhi.h"
#include "WindowRhi.h"

namespace narc_engine
{
    class SwapChainVulkan final : public SwapChainRhi
    {
    public:
        explicit SwapChainVulkan(const WindowRhiPtr window, const DeviceRhiPtr device);
        ~SwapChainVulkan() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkSwapchainKHR, getVkSwapChain, m_swapChain);

        NARC_IMPL_RHI_PLATFORM_GETTER(SwapChain, Vulkan);

    private:
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

        const std::weak_ptr<WindowRhi> m_window;
        const std::weak_ptr<DeviceRhi> m_device;
    };
} // narc_engine
