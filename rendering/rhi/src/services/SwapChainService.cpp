//
// Created by theohenno on 11/25/25.
//

#include "services/SwapChainService.h"

namespace narc_engine {
    SwapChainService::SwapChainService() = default;

    SwapChainService::~SwapChainService() = default;

    SwapChainSupportInfoVulkan SwapChainService::querySwapChainSupportInfo(const VkPhysicalDevice& physicalDevice,
                                                                           const VkSurfaceKHR& surface) const noexcept
    {
        SwapChainSupportInfoVulkan supportInfo{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &supportInfo.Capabilities);
        
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
        if (formatCount != 0)
        {
            supportInfo.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, supportInfo.Formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0)
        {
            supportInfo.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, supportInfo.PresentModes.data());
        }

        return supportInfo;
    }

    std::vector<VkImage> SwapChainService::getSwapChainImages(const VkDevice& device, const VkSwapchainKHR& swapChain) const noexcept
    {
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

        if (imageCount == 0)
        {
            return {};
        }

        std::vector<VkImage> images(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, images.data());

        return images;
    }
}
