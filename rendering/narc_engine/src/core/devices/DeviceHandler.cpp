#include "core/devices/DeviceHandler.h"

#include <NarcLog.h>

#include "core/EngineBuilder.h"
#include "core/EngineInstance.h"
#include "core/interfaces/ISurfaceProvider.h"
#include "core/QueueFamilyIndices.h"
#include "core/EngineDebugLogger.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace narc_engine
{
    DeviceHandler::DeviceHandler(const EngineBuilder* builder)
    {
        m_instance = builder->getInstance();
        m_surface = builder->getSurfaceProvider();
        m_deviceExtensions = builder->getDeviceExtensions();

        NARCLOG_DEBUG("Creating device handler...");
        m_physicalDevice = std::make_unique<PhysicalDevice>(builder);
        NARCLOG_DEBUG("Creating logical device...");
        m_logicalDevice = std::make_unique<LogicalDevice>(builder, m_physicalDevice.get());

        NARCLOG_DEBUG("Creating device properties...");
        vkGetPhysicalDeviceProperties(m_physicalDevice->getPhysicalDevice(), &m_physicalDeviceProperties);
        NARCLOG_DEBUG("Creating device queues...");
        QueueFamilyIndices indices = m_physicalDevice->getQueueFamilyIndices();
        vkGetDeviceQueue(m_logicalDevice->getVkDevice(), indices.GraphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_logicalDevice->getVkDevice(), indices.PresentFamily.value(), 0, &m_presentQueue);

    }

    DeviceHandler::~DeviceHandler()
    {
        // vkDestroyDevice(m_device, nullptr);
    }

    void DeviceHandler::setupImGui(ImGui_ImplVulkan_InitInfo* initInfo) const
    {
        initInfo->PhysicalDevice = m_physicalDevice->getPhysicalDevice();
        initInfo->Device = m_logicalDevice->getVkDevice();
        initInfo->QueueFamily = m_physicalDevice->getQueueFamilyIndices().GraphicsFamily.value();
        initInfo->Queue = m_graphicsQueue;
    }

    VkImageView DeviceHandler::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags; // VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(m_logicalDevice->getVkDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create texture image view!");
        }

        return imageView;
    }

    void DeviceHandler::waitDeviceIdle() const
    {
        if (vkDeviceWaitIdle(m_logicalDevice->getVkDevice()) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to wait for device idle!");
        }
    }

    uint32_t DeviceHandler::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice->getPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        NARCLOG_FATAL("failed to find suitable memory type!");
    }

    void DeviceHandler::createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateInfo poolInfo) const
    {
        QueueFamilyIndices queueFamilyIndices = m_physicalDevice->getQueueFamilyIndices();

        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(m_logicalDevice->getVkDevice(), &poolInfo, nullptr, commandPool) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create command pool!");
        }
    }

    VkResult DeviceHandler::submitGraphicsQueue(uint32_t submitCount, const VkSubmitInfo* submitInfo, VkFence fence) const
    {
        return vkQueueSubmit(m_graphicsQueue, submitCount, submitInfo, fence);
    }

    VkResult DeviceHandler::presentKHR(const VkPresentInfoKHR* presentInfo) const
    {
        return vkQueuePresentKHR(m_presentQueue, presentInfo);
    }

    void DeviceHandler::waitGraphicsQueueIdle() const
    {
        vkQueueWaitIdle(m_graphicsQueue);
    }

    VkFormat DeviceHandler::findDepthFormat() const
    {
        const std::vector<VkFormat> candidates = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT };

        return findSupportedFormat(candidates,
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VkFormat DeviceHandler::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
    {
        for (const VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_physicalDevice->getPhysicalDevice(), format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        NARCLOG_FATAL("Failed to find supported format!");
    }

    void DeviceHandler::createSwapChain(VkSwapchainCreateInfoKHR& createInfo, VkSwapchainKHR* swapchain) const
    {
        QueueFamilyIndices indices = m_physicalDevice->getQueueFamilyIndices();
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

        if (vkCreateSwapchainKHR(m_logicalDevice->getVkDevice(), &createInfo, nullptr, swapchain) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create swap chain!");
        }
    }

    void DeviceHandler::createLogicalDevice(const EngineDebugLogger* debugLogger)
    {

    }
}