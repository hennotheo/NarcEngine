//
// Created by theo on 12/16/25.
//

#include "vulkan_wrappers/buffers/VulkanStagingBuffer.h"

#include "vulkan_wrappers/VulkanCommandPool.h"
#include "vulkan_wrappers/VulkanQueue.h"

namespace narc_engine {
    VulkanStagingBuffer::VulkanStagingBuffer(NARC_DI_IMPORT_SERVICE(IVulkanMemoryAllocationService), NARC_DI_IMPORT_SERVICE(ICmdService)) :
        NARC_DI_IMPL_SERVICE(IVulkanMemoryAllocationService, m_allocator),
        NARC_DI_IMPL_SERVICE(ICmdService, m_cmdService)
    {
        //Empty Constructor.
    }

    VulkanStagingBuffer::~VulkanStagingBuffer() = default;

    void VulkanStagingBuffer::allocate(const VkDeviceSize& size)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        const auto query = m_allocator->allocBuffer(bufferInfo, m_vertexBuffer, m_allocation);
        if (!query.has_value())
        {
            NARC_LOG_FATAL("Vertex buffer allocation failed.");
        }

        m_allocationInfo = query.value();
    }

    void VulkanStagingBuffer::deallocate()
    {
        m_allocator->deallocBuffer(m_vertexBuffer, m_allocation);
    }

    void VulkanStagingBuffer::setData(const VulkanMemory* data)
    {
        if (m_allocation == VK_NULL_HANDLE)
        {
            NARC_LOG_FATAL("Allocation is null.");
        }

        m_allocator->mapMemory(data, m_allocationInfo.size, m_allocation);
    }

    void VulkanStagingBuffer::copyToBuffer(const IVulkanBuffer& buffer)
    {
        VkBufferCopy copy{};
        copy.size = m_allocationInfo.size;
        
        m_cmdService->doCmdActionAndSubmit([this, copy, &buffer](auto& cmd) {            
            cmd.cmdCopyBuffer(copy, *this, buffer);
        });
    }
}
