//
// Created by theo on 12/15/25.
//

#include "vulkan_wrappers/buffers/VulkanVertexBuffer.h"

#include "models/Vertex.h"

namespace narc_engine {
    VulkanVertexBuffer::VulkanVertexBuffer(NARC_DI_IMPORT_SERVICE(IVulkanMemoryAllocationService)) :
        NARC_DI_IMPL_SERVICE(IVulkanMemoryAllocationService, m_allocator)
    {
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
} // narc_engine
