//
// Created by theo on 2/23/26.
//

#include "helpers/SwapChainHelpers.h"

namespace narc_engine {

    VulkanServiceQuery<SwapChainSupportInfoVulkan> querySwapChainSupportInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
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

    VulkanServiceQuery<std::vector<VkImage>> querySwapChainImages(const VkDevice& device, const VkSwapchainKHR& swapChain)
    {
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

        if (imageCount == 0)
        {
            return vulkanServiceUnexpected("Failed to find swapchain images!");
        }

        std::vector<VkImage> images(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, images.data());

        return images;
    }
}
