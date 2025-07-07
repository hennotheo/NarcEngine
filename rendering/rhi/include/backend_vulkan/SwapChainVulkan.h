//
// Created by theoh on 28/05/2025.
//

#pragma once

#include "SwapChainRhi.h"

#include "backend_vulkan/device/DeviceVulkan.h"
#include "backend_vulkan/WindowVulkan.h"

namespace narc_engine
{
    class SwapChainVulkan final : public SwapChainRhi
    {
        using super = SwapChainRhi;

    public:
        explicit SwapChainVulkan(const WindowRhiPtr& window, const DeviceRhiPtr& device, FrameBufferRhiPtr frameBuffer);
        ~SwapChainVulkan() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkSwapchainKHR, getVkSwapChain, m_swapChain);

        NARC_IMPL_RHI_PLATFORM_GETTER(SwapChain, Vulkan);

    private:
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

        const std::weak_ptr<WindowVulkan> m_window;
        const std::weak_ptr<DeviceVulkan> m_device;
    };
} // namespace narc_engine
