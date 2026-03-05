//
// Created by theo on 12/16/25.
//

#include "buffers/VulkanStagingBuffer.h"

#include "services/VulkanMemoryAllocator.h"

namespace narc_engine {
    VulkanStagingBuffer::VulkanStagingBuffer(const VulkanMemoryAllocator* memoryAllocator, const VkDeviceSize memorySize)
        : m_allocator(memoryAllocator)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = memorySize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        const auto query = m_allocator->allocBuffer(bufferInfo, m_vertexBuffer, m_allocation);
        if (!query.has_value())
        {
            NARC_LOG_FATAL("Vertex buffer allocation failed.");
        }

        m_allocationInfo = query.value();
    }

    VulkanStagingBuffer::~VulkanStagingBuffer()
    {
        m_allocator->deallocBuffer(m_vertexBuffer, m_allocation);
    }

    void VulkanStagingBuffer::setData(const void* data)
    {
        if (m_allocation == VK_NULL_HANDLE)
        {
            NARC_LOG_FATAL("Allocation is null.");
        }

        m_allocator->mapMemory(data, m_allocationInfo.size, m_allocation);
    }
}
