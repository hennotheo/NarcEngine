//
// Created by theo on 12/17/25.
//

#include "vulkan_wrappers/buffers/VulkanIndexBuffer.h"

#include "models/Vertex.h"

namespace narc_engine {
    VulkanIndexBuffer::VulkanIndexBuffer(NARC_DI_IMPORT_SERVICE(IVulkanMemoryAllocationService)) :
    NARC_DI_IMPL_SERVICE(IVulkanMemoryAllocationService, m_allocator)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof(s_indices[0]) * s_indices.size();
        bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT ;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (!m_allocator->allocBuffer(bufferInfo, m_vertexBuffer, m_allocation))
        {
            NARC_LOG_FATAL("Index buffer allocation failed.");
        }
    }
    
    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        if (m_allocation != VK_NULL_HANDLE)
        {
            m_allocator->deallocBuffer(m_vertexBuffer, m_allocation);
        }
    }
} // narc_engine