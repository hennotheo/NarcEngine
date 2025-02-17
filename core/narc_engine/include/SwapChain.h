#pragma once

#include <vulkan/vulkan.h>
#include "include/window/Window.h"

namespace narc_engine
{
    class SwapChain
    {
    public:
        const VkExtent2D& getSwapChainExtent() const { return m_swapChainExtent; }
        const VkRenderPass& getRenderPass() const { return m_renderPass; }
        const VkSwapchainKHR& getSwapChain() const { return m_swapChain; }

        void create();
        void createFramebuffers();
        
        VkImageView createImageView(VkImage image, VkFormat format);
        VkRenderPassBeginInfo getRenderPassBeginInfos(uint32_t imageIndex) const;
        VkResult acquireNextImage(const VkSemaphore& semaphore, uint32_t* imageIndex);
        void reCreate();
        
        void cleanSwapChain();
        void cleanRenderPass();

    private:
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        VkRenderPass m_renderPass;

        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        const Window* m_window;

        void createSwapChain();
        void createRenderPass();
        void createImageViews();
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    };
}
