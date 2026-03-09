//
// Created by theohenno on 11/20/25.
//

#pragma once
#include "VulkanFramebuffer.h"

namespace narc_engine {
    class VulkanFramebuffer;
    class VulkanFence;
    class VulkanSemaphore;
    class IVulkanSurface;
    class ISwapchainService;

    class VulkanSwapChain final : public ISwapchain
    {
    public:
        explicit VulkanSwapChain(const VulkanDevice* device, const IVulkanSurface* surface, const uint32_t frameInFlightIndex);
        ~VulkanSwapChain() noexcept override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(SurfaceExtent, getSwapChainExtent, m_swapChainExtent);
        NARC_QUERY_OVERRIDE(SwapchainAcquireImageResult, acquireNextImage, const ISemaphore* semaphore, const IFence* fence);
        NARC_GETTER(VkFormat, getSwapChainImageFormat, m_swapChainImageFormat);

        NARC_GETTER(std::span<const VkImageView>, getSwapChainImageViews, m_swapChainImageViews);
        NARC_GETTER(const VulkanFramebuffer*, getFrameBuffer, &m_swapChainFrameBuffers[frameInFlightIndex],
                    const uint32_t frameInFlightIndex); //TODO: CHANGE

        NARC_GETTER(VkSwapchainKHR, getHandle, m_swapChain);

        void setRenderPass(VulkanRenderPass* renderPass);
        void setSurface(const std::unique_ptr<IVulkanSurface>& surface) { m_surface = surface.get(); }

    private:
        narc_core::injected_service<ISwapchainService> m_swapChainService;

        const VulkanDevice* m_device;
        const IVulkanSurface* m_surface;
        VulkanRenderPass* m_renderPass;

        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

        VkFormat m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
        SurfaceExtent m_swapChainExtent = {};

        std::vector<VkImage> m_swapChainImages;
        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VulkanFramebuffer> m_swapChainFrameBuffers;

        void createImageViews();
        void initFrameBuffers();

        NARC_QUERY(SwapchainAcquireImageResult, acquireNextImageImpl, const VulkanSemaphore* semaphore, const VulkanFence* fence);

        //TODO: To query
        NO_DISCARD VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
        NO_DISCARD SurfaceExtent chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
        NO_DISCARD VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
    };
} // narc_engine
