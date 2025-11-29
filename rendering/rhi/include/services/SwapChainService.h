//
// Created by theohenno on 11/25/25.
//

#pragma once

#include "VulkanSurfacesManager.h"
#include "models/SwapChainSupportInfoVulkan.h"

namespace narc_engine {    
    class SwapChainService final
    {
    public:
        SwapChainService();
        ~SwapChainService();

        //TODO: move to another service?
        NO_DISCARD SwapChainSupportInfoVulkan querySwapChainSupportInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface) const noexcept;
        NO_DISCARD std::vector<VkImage> getSwapChainImages(const VkDevice& device, const VkSwapchainKHR& swapChain) const noexcept;

    private:
        std::shared_ptr<VulkanSurfacesManager> m_surfacesManager;
    };
}
