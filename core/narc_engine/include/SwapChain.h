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
        const VkFormat& GetSwapChainImageFormat() const { return m_swapChainImageFormat; }
        const VkFramebuffer& GetSwapChainFramebuffer(uint32_t index) const { return m_swapChainFramebuffers[index]; }
        const VkExtent2D& GetSwapChainExtent() const { return m_swapChainExtent; }
        const VkSwapchainKHR& GetSwapChain() const { return m_swapChain; }

        void Create();
        void CreateFramebuffers(VkRenderPass renderPass);

        VkResult AcquireNextImage(const VkSemaphore& semaphore, const VkRenderPass& renderPass, uint32_t* imageIndex);
        void ReCreate(const VkRenderPass& renderPass);
        
        void Clean();
    private:
        VkSwapchainKHR m_swapChain;
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;
        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        Window m_window;

        void CreateSwapChain();
        void CreateImageViews();
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    };
}
