//
// Created by theo on 10/30/25.
//

#include "VulkanQueue.h"

#include "VulkanDevice.h"

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

    void VulkanQueue::assertIndexDefined(const uint32_t& index)
    {
        if (QUEUE_INDEX_NONE == index)
        {
            NARC_ERROR_RUNTIME("Queue Index is not defined.");
        }
    }
} // namespace narc_engine
