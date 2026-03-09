//
// Created by theohenno on 11/20/25.
//

#include "swapchain/VulkanSwapChain.h"

#include "device/VulkanDevice.h"
#include "helpers/SwapChainHelpers.h"
#include "mapping/mappingFromVk.h"
#include "mapping/mappingToVk.h"
#include "surface/IVulkanSurface.h"

#include "cxxabi.h"
#include "sync/VulkanFence.h"
#include "sync/VulkanSemaphore.h"
#include "VulkanFramebuffer.h"

namespace narc_engine {
    VulkanSwapChain::VulkanSwapChain(const VulkanDevice* device, const IVulkanSurface* surface, const uint32_t frameInFlightIndex) :
        m_device(device),
        m_surface(surface)
    {
    }

    VulkanSwapChain::~VulkanSwapChain() noexcept = default;

    void VulkanSwapChain::init()
    {
        NARC_GUARD_RAW_PTR(m_surface, "Surface not set for VulkanSwapChain.");
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Device.");

        const auto swapchainSupport = querySwapChainSupportInfo(m_device->getPhysicalDeviceHandle(), m_surface->getHandle())
                                      .transform_error([](const auto& err) {
                                          NARC_ERROR_RUNTIME("Surface not supported by current device");
                                          return err;
                                      }).value();

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.Formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.PresentModes);
        SurfaceExtent extent = chooseSwapExtent(swapchainSupport.Capabilities);

        uint32_t imageCount = swapchainSupport.Capabilities.minImageCount + 1;
        if (swapchainSupport.Capabilities.maxImageCount > 0 && imageCount > swapchainSupport.Capabilities.maxImageCount)
        {
            imageCount = swapchainSupport.Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface->getHandle();
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = mapping::mapFromExtend(extent);
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const auto indices = m_device->getQueueFamilyIndices();
        const std::array queueFamilyIndices = {indices.GraphicsFamily.value(), indices.PresentationFamily.value()};

        if (indices.GraphicsFamily != indices.PresentationFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapchainSupport.Capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_device->getHandle(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Could not create Vulkan Swap Chain.");
        }

        NARC_LOG_DEBUG("Vulkan SwapChain created successfully!");

        m_swapChainImages = querySwapChainImages(m_device->getHandle(), m_swapChain).transform_error([](const auto& err) {
            NARC_ERROR_RUNTIME("Surface not supported by current device");
            return err;
        }).value();
        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;

        createImageViews();
        m_renderPass->init();
        initFrameBuffers();
    }

    void VulkanSwapChain::shutdown()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Device.");

        for (auto& frameBuffer: m_swapChainFrameBuffers)
        {
            frameBuffer.shutdown();
        }

        m_renderPass->shutdown();
        for (const auto& imageView: m_swapChainImageViews)
        {
            vkDestroyImageView(m_device->getHandle(), imageView, nullptr);
        }

        vkDestroySwapchainKHR(m_device->getHandle(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
        m_swapChainImages = {};
        m_swapChainExtent = {};
        m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
    }

    SwapchainAcquireImageResult VulkanSwapChain::acquireNextImage(const ISemaphore* semaphore, const IFence* fence) const noexcept
    {
        const VulkanSemaphore* vkSemaphore = backend_cast<VulkanSemaphore>(semaphore);
        const VulkanFence* vkFence = backend_cast<VulkanFence>(fence);

        return acquireNextImageImpl(vkSemaphore, vkFence);
    }

    void VulkanSwapChain::setRenderPass(VulkanRenderPass* renderPass)
    {
        m_renderPass = renderPass;
    }

    void VulkanSwapChain::createImageViews()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Device.");

        m_swapChainImageViews.resize(m_swapChainImages.size());
        for (size_t i = 0; i < m_swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_device->getHandle(), &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
            {
                NARC_ERROR_RUNTIME("failed to create image views!");
            }
        }

        NARC_LOG_DEBUG("SwapChainImage views created successfully!");
    }

    SwapchainAcquireImageResult VulkanSwapChain::acquireNextImageImpl(const VulkanSemaphore* semaphore, const VulkanFence* fence) const noexcept
    {
        const auto vkSemaphore = semaphore != nullptr ? semaphore->getHandle() : VK_NULL_HANDLE;
        const auto vkFence = fence != nullptr ? fence->getHandle() : VK_NULL_HANDLE;

        uint32_t imageIndex = 0;
        if (const auto result = vkAcquireNextImageKHR(m_device->getHandle(), m_swapChain, UINT64_MAX, vkSemaphore, vkFence, &imageIndex);
            result != VK_SUCCESS)
        {
            return SwapchainAcquireImageResult{
                    .ImageIndex = imageIndex,
                    .HasError = true,
                    .IsOutOfDate = result == VK_ERROR_OUT_OF_DATE_KHR,
                    .IsSuboptimal = result == VK_SUBOPTIMAL_KHR
            };
        }

        return SwapchainAcquireImageResult{
                .ImageIndex = imageIndex
        };
    }

    VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
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

    SurfaceExtent VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
    {
        NARC_GUARD_RAW_PTR(m_surface, "Failed to get Surface.");

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return mapping::mapFromVkExtend(capabilities.currentExtent);
        }

        auto extend = m_surface->getSurfaceExtent();
        extend.Width = std::clamp(extend.Width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extend.Height = std::clamp(extend.Height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return extend;
    }

    void VulkanSwapChain::initFrameBuffers()
    {
        m_swapChainFrameBuffers.resize(m_swapChainImageViews.size(), VulkanFramebuffer{m_device, this});

        for (size_t i = 0; i < m_swapChainImageViews.size(); ++i)
        {
            auto& fb = m_swapChainFrameBuffers.at(i);

            fb.setRenderPass(m_renderPass);
            fb.setAttachments({m_swapChainImageViews[i]});
            fb.init();
        }
    }

    VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
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
} // narc_engine
