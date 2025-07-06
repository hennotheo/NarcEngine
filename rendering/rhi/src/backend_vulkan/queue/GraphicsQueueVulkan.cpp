//
// Created by theoh on 7/6/2025.
//

#include "backend_vulkan/queue/GraphicsQueueVulkan.h"

#include "backend_vulkan/DeviceVulkan.h"
#include "backend_vulkan/device/QueueFamilyIndicesVulkan.h"

namespace narc_engine
{
    GraphicsQueueVulkan::GraphicsQueueVulkan(const DeviceRhiPtr& device) : super(device)
    {

    }

    GraphicsQueueVulkan::~GraphicsQueueVulkan()
    {

    }

    void GraphicsQueueVulkan::init()
    {
        const QueueFamilyIndicesVulkan queueFamilyIndices = getDevice()->getPhysicalDeviceProperties().QueueFamilyIndices;

        vkGetDeviceQueue(getDevice()->getVkDevice(), queueFamilyIndices.GraphicsFamily.value(), 0, &m_queue);
    }

    void GraphicsQueueVulkan::shutdown()
    {
        m_queue = VK_NULL_HANDLE;
    }

    void GraphicsQueueVulkan::waitIdle()
    {
        if(vkQueueWaitIdle(m_queue) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to wait for graphics queue idle!");
        }
    }

    void GraphicsQueueVulkan::submit()
    {
        //TODO : Implement the submit logic
        // vkQueueSubmit(m_queue, submitCount, submitInfo, fence);
    }
} // namespace narc_engine
