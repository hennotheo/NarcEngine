//
// Created by theo on 12/16/25.
//

#pragma once

namespace narc_engine {
    class VulkanCommandPool;

    class VulkanStagingBuffer : public IVulkanBuffer
    {
    public:
        explicit VulkanStagingBuffer(NARC_DI_IMPORT_COMPONENT(IVulkanMemoryAllocationService));
        ~VulkanStagingBuffer() override;
        
        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_vertexBuffer);
        
        void allocate(const VkDeviceSize& size);
        void deallocate();
        
        void setData(const void* data, size_t size);
        void copyTo(const VulkanCommandPool &commandPool, const VulkanQueue &graphicsQueue, const IVulkanBuffer &buffer);
    
    private:
        narc_core::injected_component<IVulkanMemoryAllocationService> m_allocator;
        
        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo = {};
    };
}
