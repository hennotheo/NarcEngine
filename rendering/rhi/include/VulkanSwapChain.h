//
// Created by theohenno on 11/20/25.
//

#pragma once

#include "IVulkanSurface.h"

#include "VulkanDevice.h"

#include "services/SwapChainService.h"

namespace narc_engine {
    class PhysicalDeviceService;

    class VulkanSwapChain final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanSwapChain(std::weak_ptr<VulkanDevice> device,
                                 const std::shared_ptr<SwapChainService>& swapChainService);
        ~VulkanSwapChain() noexcept override;

        NARC_IMPL_INITIALISABLE();
        
        void setSurface(const std::weak_ptr<IVulkanSurface>& surface) { m_surface = surface; }

    private:
        std::weak_ptr<IVulkanSurface> m_surface;
        std::weak_ptr<VulkanDevice> m_device;
        std::shared_ptr<SwapChainService> m_physicalDeviceService;

        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

        //TODO: To query
        NO_DISCARD VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
        NO_DISCARD VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
        NO_DISCARD VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
    };
} // narc_engine
