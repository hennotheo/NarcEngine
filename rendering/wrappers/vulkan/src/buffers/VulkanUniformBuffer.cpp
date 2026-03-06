//
// Created by theo on 12/19/25.
//

#include "buffers/VulkanUniformBuffer.h"

#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanUniformBuffer::VulkanUniformBuffer(const VulkanMemoryAllocator* memoryAllocator, const VkDeviceSize memorySize) :
        m_allocator(memoryAllocator)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = memorySize;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        const auto query = m_allocator->allocBuffer(bufferInfo, m_buffer, m_allocation);
        if (!query.has_value())
        {
            NARC_ERROR_RUNTIME("Vertex buffer allocation failed.");
        }

        m_allocationInfo = query.value();
    }

    VulkanUniformBuffer::~VulkanUniformBuffer()
    {
        m_allocator->deallocBuffer(m_buffer, m_allocation);
        m_allocationInfo = {};
    }

    void VulkanUniformBuffer::setData(const void* data)
    {
        m_allocator->mapMemory(data, m_allocationInfo.size, m_allocation);
    }
} // narc_engine
