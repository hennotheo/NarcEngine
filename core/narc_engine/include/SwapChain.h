#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

#include "window/Window.h"

namespace NarcEngine
{
    class SwapChain
    {
    public:
        const VkExtent2D& GetSwapChainExtent() const { return m_swapChainExtent; }
        const VkRenderPass& GetRenderPass() const { return m_renderPass; }
        const VkSwapchainKHR& GetSwapChain() const { return m_swapChain; }

        void Create();
        void CreateFramebuffers();
        
        VkRenderPassBeginInfo GetRenderPassBeginInfos(uint32_t imageIndex) const;
        VkResult AcquireNextImage(const VkSemaphore& semaphore, uint32_t* imageIndex);
        void ReCreate();
        
        void CleanSwapChain();
        void CleanRenderPass();

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
        Window m_window;

        void CreateSwapChain();
        void CreateRenderPass();
        void CreateImageViews();
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    };
}
