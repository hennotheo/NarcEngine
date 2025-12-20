//
// Created by theo on 12/19/25.
//

#include "vulkan_wrappers/buffers/VulkanUniformBuffer.h"

#include "vulkan_wrappers/VulkanDevice.h"

namespace narc_engine {
    VulkanUniformBuffer::VulkanUniformBuffer(NARC_DI_IMPORT_COMPONENT(IVulkanMemoryAllocationService)) :
        NARC_DI_IMPL_COMPONENT(IVulkanMemoryAllocationService, m_allocator)
    {
    }

    VulkanUniformBuffer::~VulkanUniformBuffer()
    {
    }

    void VulkanUniformBuffer::allocate(const VkDeviceSize& size)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (const auto query = m_allocator->allocBuffer(bufferInfo, m_buffer, m_allocation); !query.has_value())
        {
            NARC_LOG_FATAL("Vertex buffer allocation failed.");
        }
    }

    void VulkanUniformBuffer::deallocate()
    {
        m_allocator->deallocBuffer(m_buffer, m_allocation);
    }

    void VulkanUniformBuffer::setData(const VkDeviceSize& size, const void* data)
    {
        m_allocator->mapMemory(data, size, m_allocation);
    }
} // narc_engine
