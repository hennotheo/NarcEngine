//
// Created by theohenno on 11/20/25.
//

#pragma once

namespace narc_engine {
    class DeviceService;
    class ISwapchainService;

    class VulkanSwapChain final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanSwapChain(std::weak_ptr<VulkanDevice> device,
                                 NARC_DI_IMPORT_SERVICE(ISwapchainService));
        ~VulkanSwapChain() noexcept override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(SurfaceExtend, getSwapChainExtent, m_swapChainExtent);
        NARC_GETTER(VkFormat, getSwapChainImageFormat, m_swapChainImageFormat);

        NARC_GETTER(std::span<const VkImageView>, getSwapChainImageViews, m_swapChainImageViews);
        
        NARC_GETTER(VkSwapchainKHR, getHandle, m_swapChain);
        
        void setSurface(const std::unique_ptr<IWindow>& surface) { m_surface = surface.get(); }

    private:
        narc_core::injected_service<ISwapchainService> m_swapChainService;
        
        std::weak_ptr<VulkanDevice> m_device;
        IWindow* m_surface;

        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

        VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
        SurfaceExtend m_swapChainExtent = {};

        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;

        void createImageViews();

        //TODO: To query
        NO_DISCARD VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
        NO_DISCARD SurfaceExtend chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
        NO_DISCARD VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
    };
} // narc_engine
