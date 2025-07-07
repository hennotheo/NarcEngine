//
// Created by theoh on 28/05/2025.
//

#include "backend_vulkan/SwapChainVulkan.h"

namespace narc_engine
{
    SwapChainVulkan::SwapChainVulkan(const WindowRhiPtr& window, const DeviceRhiPtr& device, FrameBufferRhiPtr frameBuffer) :
        super(frameBuffer),
        m_window(std::static_pointer_cast<WindowVulkan>(window)),
        m_device(std::static_pointer_cast<DeviceVulkan>(device))
    {
    }

    SwapChainVulkan::~SwapChainVulkan() = default;

    void SwapChainVulkan::init() //TODO : TOO LONG, REFACTOR
    {
        NARC_GUARD_WEAK(window, m_window, "Window is null!");
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        const auto vulkanDeviceProps = device->getPhysicalDeviceProperties();

        const auto& swapChainSupport = vulkanDeviceProps.SwapChainSupportDetails;
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
        createInfo.surface = window->getVkSurface();
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

        const QueueFamilyIndicesVulkan indices = vulkanDeviceProps.QueueFamilyIndices;
        const std::array queueFamilyIndices = {indices.GraphicsFamily.value(), indices.PresentFamily.value()};

        if (indices.GraphicsFamily != indices.PresentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Multiple queue family without explicit ownership
            createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Best perf : one queue family ownership
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        if (vkCreateSwapchainKHR(device->getVkDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create swap chain!");
        }
    }

    void SwapChainVulkan::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        vkDestroySwapchainKHR(device->getVkDevice(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
    }
} // namespace narc_engine
