//
// Created by theohenno on 11/20/25.
//

#include "VulkanSwapChain.h"

#include <GLFW/glfw3.h>

#include <utility>

#include "IVulkanSurface.h"
#include "VulkanDevice.h"
#include "services/PhysicalDeviceService.h"
#include "services/SwapChainService.h"

namespace narc_engine {
    VulkanSwapChain::VulkanSwapChain(std::weak_ptr<VulkanDevice> device, const std::shared_ptr<SwapChainService>& swapChainService) :
        m_device(std::move(device)),
        m_physicalDeviceService(swapChainService)
    {
    }

    VulkanSwapChain::~VulkanSwapChain() noexcept = default;

    void VulkanSwapChain::init()
    {
        NARC_GUARD_WEAK(surfaceHandler, m_surface, "Failed to get Surface");
        NARC_GUARD_WEAK(device, m_device, "Failed to get Device");
        const auto surface = surfaceHandler->getHandled();

        const SwapChainSupportInfoVulkan swapChainSupport = m_physicalDeviceService->querySwapChainSupportInfo(
                device->getPhysicalDeviceHandle(), surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.Formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.PresentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.Capabilities);

        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;
        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const auto indices = device->getQueueFamilyIndices();
        const std::array queueFamilyIndices = {indices.GraphicsFamily.value(), indices.PresentationFamily.value()};

        if (indices.GraphicsFamily != indices.PresentationFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device->getHandle(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swap chain!");
        }
    }

    void VulkanSwapChain::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Device.");

        vkDestroySwapchainKHR(device->getHandle(), m_swapChain, nullptr);
    }

    VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
    {
        for (const auto& availablePresentMode: availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
    {
        NARC_GUARD_WEAK(surface, m_surface, "Failed to get Surface");

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        auto extend = surface->getSurfaceExtent();
        extend.width = std::clamp(extend.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extend.height = std::clamp(extend.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return extend;
    }

    VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
    {
        for (const auto& availableFormat: availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }
} // narc_engine
