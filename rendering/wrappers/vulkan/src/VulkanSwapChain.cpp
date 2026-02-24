//
// Created by theohenno on 11/20/25.
//

#include "swapchain/VulkanSwapChain.h"

#include "device/VulkanDevice.h"
#include "mapping/mappingFromVk.h"
#include "mapping/mappingToVk.h"
#include "services/SwapChainService.h"

namespace narc_engine {
    VulkanSwapChain::VulkanSwapChain(std::weak_ptr<VulkanDevice> device, NARC_DI_IMPORT_SERVICE(ISwapchainService)) :
        NARC_DI_IMPL_SERVICE(ISwapchainService, m_swapChainService),
        m_device(std::move(device)),
        m_surface(nullptr)
    {
    }

    VulkanSwapChain::~VulkanSwapChain() noexcept = default;

    void VulkanSwapChain::init()
    {
        if (m_surface == nullptr)
        {
            NARC_ERROR_RUNTIME("Surface not set for VulkanSwapChain.");
        }
        
        NARC_GUARD_WEAK(device, m_device, "Failed to get Device");
        const auto surface = nullptr;
        NARC_ERROR_NOT_IMPLEMENTED("Surface is not defined");

        const auto swapchainSupport = m_swapChainService->querySwapChainSupportInfo(device->getPhysicalDeviceHandle(), surface)
                                                        .transform_error([](const auto& err) {
                                                            NARC_ERROR_RUNTIME("Surface not supported by current device");
                                                            return err;
                                                        }).value();

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupport.Formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupport.PresentModes);
        SurfaceExtend extent = chooseSwapExtent(swapchainSupport.Capabilities);

        uint32_t imageCount = swapchainSupport.Capabilities.minImageCount + 1;
        if (swapchainSupport.Capabilities.maxImageCount > 0 && imageCount > swapchainSupport.Capabilities.maxImageCount)
        {
            imageCount = swapchainSupport.Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = mapping::mapFromExtend(extent);
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const auto indices = device->getQueueFamilyIndices();
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

        if (vkCreateSwapchainKHR(device->getHandle(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Could not create Vulkan Swap Chain.");
        }

        m_swapChainImages = m_swapChainService->querySwapChainImages(device->getHandle(), m_swapChain).transform_error([](const auto& err) {
            NARC_ERROR_RUNTIME("Surface not supported by current device");
            return err;
        }).value();
        m_swapChainImageFormat = surfaceFormat.format;
        m_swapChainExtent = extent;

        createImageViews();
    }

    void VulkanSwapChain::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Device.");

        for (auto imageView: m_swapChainImageViews)
        {
            vkDestroyImageView(device->getHandle(), imageView, nullptr);
        }

        vkDestroySwapchainKHR(device->getHandle(), m_swapChain, nullptr);
        m_swapChain = VK_NULL_HANDLE;
        m_swapChainImages = {};
        m_swapChainExtent = {};
        m_swapChainImageFormat = VK_FORMAT_UNDEFINED;
    }

    void VulkanSwapChain::createImageViews()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Device.");

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

            if (vkCreateImageView(device->getHandle(), &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
            {
                NARC_ERROR_RUNTIME("failed to create image views!");
            }
        }
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

    SurfaceExtend VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
    {
        if (m_surface == nullptr)
        {
            NARC_ERROR_RUNTIME("Surface not set for VulkanSwapChain.");
        }

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return mapping::mapFromVkExtend(capabilities.currentExtent);
        }

        auto extend = m_surface->getSurfaceExtent();
        extend.Width = std::clamp(extend.Width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extend.Height = std::clamp(extend.Height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return extend;
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
