//
// Created by theoh on 31/05/2025.
//

#pragma once

#include "WindowRhi.h"

namespace narc_engine
{
    struct SwapChainSupportDetailsVulkan
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;

        TEMP_CODE inline VkPresentModeKHR chooseSwapPresentMode() const
        {
            if (PresentModes.empty())
            {
                NARCLOG_FATAL("No available present modes for swapchain!");
            }

            for (const auto& availablePresentMode : PresentModes)
            {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    return availablePresentMode;
                }
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }

        TEMP_CODE inline VkExtent2D chooseSwapExtent(const WindowRhi* surface) const
        {
            if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                return Capabilities.currentExtent;
            }

            const glm::u32vec2 size = surface->getFramebufferSize();

            VkExtent2D actualExtent =
            {
                size.x,
                size.y
            };

            actualExtent.width = std::clamp(actualExtent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

            return actualExtent;
        }

        TEMP_CODE inline VkSurfaceFormatKHR chooseSwapSurfaceFormat()
        {
            if (Formats.empty())
            {
                NARCLOG_FATAL("No available formats for swapchain!");
            }
            for (const auto& availableFormat : Formats)
            {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return availableFormat;
                }
            }

            return Formats[0];
        }
    };
}
