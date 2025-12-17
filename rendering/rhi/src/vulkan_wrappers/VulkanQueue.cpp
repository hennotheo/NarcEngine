//
// Created by theo on 10/30/25.
//

#include "vulkan_wrappers/VulkanQueue.h"

#include "vulkan_wrappers/VulkanDevice.h"
#include "vulkan_wrappers/VulkanFence.h"

namespace narc_engine {
    VulkanQueue::VulkanQueue() = default;
    VulkanQueue::~VulkanQueue() = default;

    void VulkanQueue::init()
    {
        assertIndexDefined(m_queueIndex);
        assertIndexDefined(m_queueFamilyIndex);

        NARC_GUARD_WEAK(device, m_device, "Device not defined.");

        vkGetDeviceQueue(device->getHandle(), m_queueFamilyIndex, m_queueIndex, &m_queue);
    }

    void VulkanQueue::shutdown() { m_queue = VK_NULL_HANDLE; }

    VkResult VulkanQueue::submit(const uint32_t submitCount, const VkSubmitInfo& infos, const VulkanFence* fence) const
    {
        VkFence pfence = fence == nullptr ? VK_NULL_HANDLE : fence->getHandle();
        
        return vkQueueSubmit(m_queue, submitCount, &infos, pfence);
    }

    void VulkanQueue::waitIdle() const
    {
        vkQueueWaitIdle(m_queue);
    }

    void VulkanQueue::assertIndexDefined(const uint32_t& index)
    {
        if (QUEUE_INDEX_NONE == index)
        {
            NARC_ERROR_RUNTIME("Queue Index is not defined.");
        }
    }
} // namespace narc_engine
