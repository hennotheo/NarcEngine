//
// Created by theo on 12/15/25.
//

#include "buffers/VulkanVertexBuffer.h"
#include "services/VulkanMemoryAllocator.h"

namespace narc_engine {
    VulkanVertexBuffer::VulkanVertexBuffer(const VulkanMemoryAllocator* memoryAllocator) :
        m_allocator(memoryAllocator)
    {
        //TODO: DISGUSTING HARD CODE
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(s_vertices[0]) * s_vertices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (!m_allocator->allocBuffer(bufferInfo, m_vertexBuffer, m_allocation))
        {
            NARC_LOG_FATAL("Vertex buffer allocation failed.");
        }
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        m_allocator->deallocBuffer(m_vertexBuffer, m_allocation);
    }

    void VulkanVertexBuffer::setData(const void* data)
    {
        if (m_allocation == VK_NULL_HANDLE)
        {
            NARC_LOG_FATAL("Allocation is null.");
        }

        m_allocator->mapMemory(data, m_allocationInfo.size, m_allocation);
    }
} // narc_engine
