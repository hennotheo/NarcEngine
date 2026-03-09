//
// Created by theo on 12/17/25.
//

#include "buffers/VulkanIndexBuffer.h"
#include "services/VulkanMemoryAllocator.h"

namespace narc_engine {
    VulkanIndexBuffer::VulkanIndexBuffer(const VulkanMemoryAllocator* memoryAllocator, const MemorySize size)
        : m_allocator(memoryAllocator)
    {
        //TODO: DISGUSTING HARD CODE
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        const auto query = m_allocator->allocBuffer(bufferInfo, m_vertexBuffer, m_allocation);
        if (!query.has_value())
        {
            NARC_LOG_FATAL("Index buffer allocation failed.");
        }

        m_allocationInfo = query.value();
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        m_allocator->deallocBuffer(m_vertexBuffer, m_allocation);
    }

    void VulkanIndexBuffer::setData(const void* data)
    {
        if (m_allocation == VK_NULL_HANDLE)
        {
            NARC_LOG_FATAL("Allocation is null.");
        }

        m_allocator->mapMemory(data, m_allocationInfo.size, m_allocation);
    }
} // narc_engine
