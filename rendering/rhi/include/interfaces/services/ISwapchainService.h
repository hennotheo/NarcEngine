//
// Created by theohenno on 12/3/25.
//

#pragma once

namespace narc_engine {
    struct SwapChainSupportInfoVulkan;
    
    class ISwapchainService : public IService
    {
    public:
        ~ISwapchainService() override = default;

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<SwapChainSupportInfoVulkan>, querySwapChainSupportInfo,
                                const VkPhysicalDevice& physicalDevice,
                                const VkSurfaceKHR& surface);

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<std::vector<VkImage>>, querySwapChainImages,
                                const VkDevice& device,
                                const VkSwapchainKHR& swapChain);
    };
}
