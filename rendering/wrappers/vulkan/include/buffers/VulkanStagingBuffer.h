//
// Created by theo on 12/16/25.
//

#pragma once

namespace narc_engine {
    class VulkanMemoryAllocator;

    class VulkanStagingBuffer : public IVulkanBuffer
    {
    public:
        explicit VulkanStagingBuffer(const VulkanMemoryAllocator* memoryAllocator, VkDeviceSize memorySize);
        ~VulkanStagingBuffer() override;
        
        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_vertexBuffer);

        void setData(const void* data) override;
    
    private:
        const VulkanMemoryAllocator* m_allocator;

        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo = {};
    };
}
