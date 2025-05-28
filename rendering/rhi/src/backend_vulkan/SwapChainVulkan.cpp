//
// Created by theoh on 28/05/2025.
//

#include "backend_vulkan/SwapChainVulkan.h"

#include "backend_vulkan/DeviceVulkan.h"
#include "backend_vulkan/WindowVulkan.h"

namespace narc_engine
{
    SwapChainVulkan::SwapChainVulkan(const WindowRhi* window, const DeviceRhi* device) :
        SwapChainRhi(window, device)
    {
    }

    SwapChainVulkan::~SwapChainVulkan()
    {
    }

    void SwapChainVulkan::init()
    {

    }

    void SwapChainVulkan::shutdown()
    {
        vkDestroySwapchainKHR(m_device->getDeviceVulkan()->getVkDevice(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
    }

} // narc_engine
