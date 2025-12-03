//
// Created by theohenno on 11/25/25.
//

#pragma once

#include "models/SwapChainSupportInfoVulkan.h"

namespace narc_engine {    
    class SwapChainService final : public ISwapchainService
    {
    public:
        SwapChainService();
        ~SwapChainService() override;

        NARC_QUERY_OVERRIDE(VulkanServiceQuery<SwapChainSupportInfoVulkan>, querySwapChainSupportInfo,
                                const VkPhysicalDevice& physicalDevice,
                                const VkSurfaceKHR& surface);

        NARC_QUERY_OVERRIDE(VulkanServiceQuery<std::vector<VkImage>>, querySwapChainImages,
                        const VkDevice& device,
                        const VkSwapchainKHR& swapChain);
    };
}
