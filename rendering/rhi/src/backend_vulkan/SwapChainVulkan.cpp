//
// Created by theoh on 28/05/2025.
//

#include "backend_vulkan/SwapChainVulkan.h"

#include "backend_vulkan/FrameBufferVulkan.h"
#include "backend_vulkan/resources/ImageViewVulkan.h"
#include "backend_vulkan/resources/ImageVulkan.h"

namespace narc_engine
{
    SwapChainVulkan::SwapChainVulkan(const WindowRhi* window, const DeviceRhiPtr& device) :
        super(window, device)
    {
    }

    SwapChainVulkan::~SwapChainVulkan() = default;

    void SwapChainVulkan::createSwapChain()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");
        NARC_GUARD_RAW_PTR(m_window, "Window is not compatible with Device!");

        const auto deviceVulkan = device->getDeviceVulkan();
        const auto windowVulkan = m_window->getWindowVulkan();

        const auto vulkanDeviceProps = deviceVulkan->getPhysicalDeviceProperties();

        const auto& swapChainSupport = vulkanDeviceProps.SwapChainSupportDetails;
        const VkSurfaceFormatKHR surfaceFormat = swapChainSupport.chooseSwapSurfaceFormat();
        const VkPresentModeKHR presentMode = swapChainSupport.chooseSwapPresentMode();
        m_extent = swapChainSupport.chooseSwapExtent(*m_window);

        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1; // Min + 1 to allow for double buffering

        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = windowVulkan->getVkSurface();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = m_extent;
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

        if (vkCreateSwapchainKHR(deviceVulkan->getVkDevice(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create swap chain!");
        }
    }

    void SwapChainVulkan::cleanupSwapChain()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        vkDestroySwapchainKHR(device->getDeviceVulkan()->getVkDevice(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
    }

    void SwapChainVulkan::createImages()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        const auto& vkDevice = device->getDeviceVulkan()->getVkDevice();

        uint32_t imageCount = 0;
        vkGetSwapchainImagesKHR(vkDevice, m_swapChain, &imageCount, nullptr);

        m_images.resize(imageCount);
        std::vector<VkImage> vkImages(imageCount);
        vkGetSwapchainImagesKHR(vkDevice, m_swapChain, &imageCount, vkImages.data());

        for (uint32_t i = 0; i < imageCount; ++i)
        {
            m_images[i] = std::make_shared<SwapChainImageVulkan>(vkImages[i]);
        }
    }

    void SwapChainVulkan::createImageViews()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        m_imageViews.resize(m_images.size());
        for (size_t i = 0; i < m_images.size(); i++)
        {
            if (m_imageViews[i] != nullptr)
            {
                continue;
            }

            m_imageViews[i] = std::make_shared<ImageViewVulkan>(device, m_images[i]->getImageVulkan(), m_format);
            m_imageViews[i]->init();
        }
    }

    void SwapChainVulkan::createFramebuffers()
    {
        // NARC_GUARD_RAW_PTR(m_window, "Window is null!");
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        m_framebuffers.resize(m_images.size());

        for (auto& framebuffer : m_framebuffers)
        {
            if (framebuffer == nullptr)
            {
                framebuffer = std::make_shared<FrameBufferVulkan>(device, this);
            }

            framebuffer->init();
        }
    }
} // namespace narc_engine
