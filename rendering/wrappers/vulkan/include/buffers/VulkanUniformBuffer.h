//
// Created by theo on 12/19/25.
//

#pragma once

namespace narc_engine {
    class VulkanMemoryAllocator;

    class VulkanUniformBuffer  : public IVulkanBuffer {
    public:
        explicit VulkanUniformBuffer(const VulkanMemoryAllocator* memoryAllocator, VkDeviceSize memorySize);
        ~VulkanUniformBuffer() override;
        
        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_buffer);
        NARC_OVERRIDE_GETTER(MemorySize, getSize, m_allocationInfo.size)
        void setData(const void* data) override;

    private:
        const VulkanMemoryAllocator* m_allocator;
        
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo{};
    };
} // narc_engine
