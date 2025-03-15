#include "renderer/SwapChain.h"

#include <NarcLog.h>

#include "Engine.h"
#include "core/DeviceHandler.h"
#include "renderer/DepthResources.h"

namespace narc_engine {
    void SwapChain::create()
    {
        const Engine* engine = Engine::getInstance();
        m_window = engine->getWindow();

        createSwapChain();
        createImageViews();
        m_renderPass = std::make_unique<RenderPass>(m_swapChainImageFormat);
        m_depthResources = std::make_unique<DepthResources>();
        m_depthResources->create(m_swapChainExtent.width, m_swapChainExtent.height);
    }

    VkResult SwapChain::acquireNextImage(const VkSemaphore& semaphore, uint32_t* imageIndex)
    {
        const VkResult result = vkAcquireNextImageKHR(getVkDevice(), m_swapChain, UINT64_MAX, semaphore, VK_NULL_HANDLE, imageIndex);

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
        for (auto framebuffer: m_swapChainFramebuffers)
        {
            vkDestroyFramebuffer(getVkDevice(), framebuffer, nullptr);
        }

        for (auto imageView: m_swapChainImageViews)
        {
            vkDestroyImageView(getVkDevice(), imageView, nullptr);
        }

        vkDestroySwapchainKHR(getVkDevice(), m_swapChain, nullptr);
    }

    void SwapChain::cleanRenderPass()
    {
        m_renderPass.reset();
    }

    void SwapChain::reCreate()
    {
        int width = 0;
        int height = 0;
        m_window->getValidFramebufferSize(&width, &height);

        getDeviceHandler()->waitDeviceIdle();

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
                m_swapChainImageViews[i],
                m_depthResources->getImageView()
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = getRenderPass()->getRenderPass();
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_swapChainExtent.width;
            framebufferInfo.height = m_swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(getVkDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
            {
                NARCLOG_FATAL("failed to create framebuffer!");
            }
        }
    }

    VkRenderPassBeginInfo SwapChain::getRenderPassBeginInfos(uint32_t imageIndex) const
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = getRenderPass()->getRenderPass();
        renderPassInfo.framebuffer = m_swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_swapChainExtent;

        return renderPassInfo;
    }

    void SwapChain::createSwapChain()
    {
        SwapChainSupportDetails swapChainSupport = m_window->querySwapChainSupport(getDeviceHandler()->getPhysicalDevice());
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.Formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.PresentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.Capabilities);

        uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1; //Au moins une en plus pour eviter des erreurs

        if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_window->getSurface();
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

        getDeviceHandler()->createSwapChain(createInfo, &m_swapChain);

        vkGetSwapchainImagesKHR(getVkDevice(), m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(getVkDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;
    }

    void SwapChain::createImageViews()
    {
        m_swapChainImageViews.resize(m_swapChainImages.size());

        for (size_t i = 0; i < m_swapChainImages.size(); i++)
        {
            m_swapChainImageViews[i] = getDeviceHandler()->createImageView(m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat: availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode: availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        int width, height;
        m_window->getFramebufferSize(&width, &height);

        VkExtent2D actualExtent =
        {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
