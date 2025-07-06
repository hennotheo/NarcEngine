//
// Created by theoh on 7/6/2025.
//

#include "backend_vulkan/queue/PresentQueueVulkan.h"

namespace narc_engine
{
    PresentQueueVulkan::PresentQueueVulkan(const DeviceRhiPtr& device) : super(device)
    {

    }

    PresentQueueVulkan::~PresentQueueVulkan()
    {
    }

    void PresentQueueVulkan::present() const
    {
        //TODO : Implement the present logic
        // vkQueuePresentKHR(m_queue, &presentInfo);
    }

    void PresentQueueVulkan::init()
    {
        const QueueFamilyIndicesVulkan queueFamilyIndices = getDevice()->getPhysicalDeviceProperties().QueueFamilyIndices;

        vkGetDeviceQueue(getDevice()->getVkDevice(), queueFamilyIndices.PresentFamily.value(), 0, &m_queue);
    }

    void PresentQueueVulkan::shutdown()
    {
        m_queue = VK_NULL_HANDLE;
    }
} // narc_engine
