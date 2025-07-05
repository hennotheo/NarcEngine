#include "platform/vulkan/SwapChain.h"

#include "Engine.h"
#include "core/devices/DeviceHandler.h"
#include "renderer/DepthResources.h"
#include "core/interfaces/ISurfaceProvider.h"

#include "platform/vulkan/sync/Semaphore.h"

namespace narc_engine {
    SwapChain::SwapChain()
    {
    }

    SwapChain::~SwapChain()
    {
    }
    
    void SwapChain::create(ISurfaceProvider* surface)
    {
        m_surface = surface;

        createSwapChain();
        createImageViews();
        m_renderPass = std::make_unique<RenderPass>(m_swapChainImageFormat);
        m_depthResources = std::make_unique<DepthResources>();
        m_depthResources->create(m_swapChainExtent.width, m_swapChainExtent.height);
    }

    VkResult SwapChain::acquireNextImage(const Semaphore* semaphore, uint32_t* imageIndex)
    {
        const VkResult result = vkAcquireNextImageKHR(NARC_DEVICE_HANDLE, m_swapChain, UINT64_MAX, semaphore->get(), VK_NULL_HANDLE, imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) //OUT DUE TO WINDOW RESIZE FOR EXAMPLE
        {
            reCreate();
            return result;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            NARCLOG_FATAL("failed to acquire swap chain image!");
        }

        return result;
    }

    void SwapChain::cleanSwapChain()
    {
        m_depthResources->release();
        for (auto framebuffer : m_swapChainFramebuffers)
        {
            vkDestroyFramebuffer(NARC_DEVICE_HANDLE, framebuffer, nullptr);
        }

        for (auto imageView : m_swapChainImageViews)
        {
            imageView.release();
        }

        vkDestroySwapchainKHR(NARC_DEVICE_HANDLE, m_swapChain, nullptr);
    }

    void SwapChain::cleanRenderPass()
    {
        m_renderPass.reset();
    }

    void SwapChain::reCreate()
    {
        int width = 0;
        int height = 0;
        m_surface->getValidFramebufferSize(&width, &height);

        if (width == 0 || height == 0)
        {
            NARCLOG_FATAL("Window size is 0!");
        }

        NARC_DEVICE->waitDeviceIdle();

        cleanSwapChain();

        createSwapChain();
        createImageViews();
        m_depthResources->create(m_swapChainExtent.width, m_swapChainExtent.height);
        createFramebuffers();
    }

    void SwapChain::createFramebuffers()
    {
        m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

        for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
        {
            std::array<VkImageView, 2> attachments = {
                m_swapChainImageViews[i].get(),
                m_depthResources->getImageView()->get()
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = getRenderPass()->get();
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_swapChainExtent.width;
            framebufferInfo.height = m_swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(NARC_DEVICE_HANDLE, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
            {
                NARCLOG_FATAL("failed to create framebuffer!");
            }
        }
    }

    void SwapChain::createSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = NARC_PHYSICAL_DEVICE->querySwapChainSupport();
        VkSurfaceFormatKHR surfaceFormat = swapChainSupport.chooseSwapSurfaceFormat();
        VkPresentModeKHR presentMode = swapChainSupport.chooseSwapPresentMode();
        VkExtent2D extent = swapChainSupport.chooseSwapExtent(m_surface);

        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1; //Au moins une en plus pour eviter des erreurs

        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface->get();
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

        QueueFamilyIndices indices = NARC_PHYSICAL_DEVICE->getQueueFamilyIndices();
        uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

        if (indices.GraphicsFamily != indices.PresentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Multiple queue family without explicit ownership
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Best perf : one queue family ownership
            createInfo.queueFamilyIndexCount = 0;                    // Optional
            createInfo.pQueueFamilyIndices = nullptr;                // Optional
        }

        if (vkCreateSwapchainKHR(NARC_DEVICE_HANDLE, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(NARC_DEVICE_HANDLE, m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(NARC_DEVICE_HANDLE, m_swapChain, &imageCount, m_swapChainImages.data());

        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;
    }

    void SwapChain::createImageViews()
    {
        m_swapChainImageViews.resize(m_swapChainImages.size());

        for (size_t i = 0; i < m_swapChainImages.size(); i++)
        {
            m_swapChainImageViews[i].create(m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }
}
