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

        m_physicalDevice = std::make_unique<PhysicalDevice>(builder);
        m_logicalDevice = std::make_unique<LogicalDevice>(builder, m_physicalDevice.get());

        QueueFamilyIndices indices = m_physicalDevice->getQueueFamilyIndices();
        vkGetDeviceQueue(m_logicalDevice->getVkDevice(), indices.GraphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_logicalDevice->getVkDevice(), indices.PresentFamily.value(), 0, &m_presentQueue);
    }

    DeviceHandler::~DeviceHandler()
    {
        
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
}