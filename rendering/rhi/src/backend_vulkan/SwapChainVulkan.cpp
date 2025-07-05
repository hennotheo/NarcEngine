//
// Created by theoh on 28/05/2025.
//

#include "backend_vulkan/SwapChainVulkan.h"

#include "backend_vulkan/DeviceVulkan.h"
#include "backend_vulkan/WindowVulkan.h"

namespace narc_engine
{
    SwapChainVulkan::SwapChainVulkan(const WindowRhiPtr window, const DeviceRhiPtr device) : m_window(window), m_device(device) {}

    SwapChainVulkan::~SwapChainVulkan() {}

    void SwapChainVulkan::init()
    {
        const auto window = m_window.lock();
        if (!window)
        {
            NARCLOG_FATAL("SwapChainVulkan: Window is not initialized.");
        }

        const auto device = m_device.lock();
        if (!device)
        {
            NARCLOG_FATAL("SwapChainVulkan: Device is not initialized.");
        }

        const PhysicalDeviceVulkanProperties props = device->getDeviceVulkan()->getPhysicalDeviceProperties();

        SwapChainSupportDetailsVulkan swapChainSupport = props.SwapChainSupportDetails;
        const VkSurfaceFormatKHR surfaceFormat = swapChainSupport.chooseSwapSurfaceFormat();
        const VkPresentModeKHR presentMode = swapChainSupport.chooseSwapPresentMode();
        const VkExtent2D extent = swapChainSupport.chooseSwapExtent(*window);

        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1; // Min + 1 to allow for double buffering

        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = window->getWindowVulkan()->getVkSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        const QueueFamilyIndicesVulkan indices = props.QueueFamilyIndices;
        const uint32_t queueFamilyIndices[] = {indices.GraphicsFamily.value(), indices.PresentFamily.value()};

        if (indices.GraphicsFamily != indices.PresentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Multiple queue family without explicit ownership
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Best perf : one queue family ownership
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        if (vkCreateSwapchainKHR(device->getDeviceVulkan()->getVkDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create swap chain!");
        }
    }

    void SwapChainVulkan::shutdown()
    {
        const auto device = m_device.lock();
        if (!device)
        {
            NARCLOG_FATAL("SwapChainVulkan: Window is not initialized.");
        }

        vkDestroySwapchainKHR(device->getDeviceVulkan()->getVkDevice(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
    }

} // namespace narc_engine
