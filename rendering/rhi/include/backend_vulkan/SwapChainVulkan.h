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
        explicit SwapChainVulkan(const WindowRhi* window, const DeviceRhiPtr& device);
        ~SwapChainVulkan() override;

        NARC_GETTER(VkSwapchainKHR, getVkSwapChain, m_swapChain);
        NARC_OVERRIDE_GETTER(SwapChainExtends, getExtends, SwapChainExtends(m_extent.width, m_extent.height));

        NARC_IMPL_RHI_PLATFORM_GETTER(SwapChain, Vulkan);

    private:
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
        VkExtent2D m_extent{};

        void createSwapChain() override;

    protected:
        void cleanupSwapChain() override;
        void createImages() override;
        void createImageViews() override;
        void createFramebuffers() override;
    };
} // namespace narc_engine
