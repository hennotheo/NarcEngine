//
// Created by theo on 12/16/25.
//

#pragma once

namespace narc_engine {
    class VulkanCommandPool;

    class VulkanStagingBuffer : public IVulkanBuffer
    {
    public:
        explicit VulkanStagingBuffer(NARC_DI_IMPORT_SERVICE(IVulkanMemoryAllocationService), NARC_DI_IMPORT_SERVICE(ICmdService));
        ~VulkanStagingBuffer() override;
        
        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_vertexBuffer);
        
        void allocate(const VkDeviceSize& size);
        void deallocate();
        
        void setData(const Memory* data);
        void copyToBuffer(const IVulkanBuffer &buffer);
    
    private:
        narc_core::injected_component<IVulkanMemoryAllocationService> m_allocator;
        narc_core::injected_component<ICmdService> m_cmdService;
        
        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo = {};
    };
}
