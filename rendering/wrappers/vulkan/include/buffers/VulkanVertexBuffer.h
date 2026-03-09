//
// Created by theo on 12/15/25.
//

#pragma once

#include <vk_mem_alloc.h>

namespace narc_engine {
    class VulkanMemoryAllocator;

    class VulkanVertexBuffer : public IVulkanBuffer
    {
    public:
        explicit VulkanVertexBuffer(const VulkanMemoryAllocator* memoryAllocator, MemorySize size);
        ~VulkanVertexBuffer() override;

        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_vertexBuffer)
        NARC_OVERRIDE_GETTER(MemorySize, getSize, m_allocationInfo.size);
        void setData(const void* data) override;

    private:
        const VulkanMemoryAllocator* m_allocator;

        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo = {};
    };
} // narc_engine
