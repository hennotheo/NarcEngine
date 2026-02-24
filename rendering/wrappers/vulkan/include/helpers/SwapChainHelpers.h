//
// Created by theo on 2/23/26.
//

#pragma once

#include "models/SwapChainSupportInfoVulkan.h"

namespace narc_engine {
    NO_DISCARD VulkanServiceQuery<SwapChainSupportInfoVulkan> querySwapChainSupportInfo(
            const VkPhysicalDevice& physicalDevice,
            const VkSurfaceKHR& surface);
}
