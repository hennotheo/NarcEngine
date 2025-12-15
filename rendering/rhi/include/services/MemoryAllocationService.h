//
// Created by theo on 12/15/25.
//

#pragma once

#include <vk_mem_alloc.h>

namespace narc_engine {
    class VulkanInstance;

    class MemoryAllocationService : public IVulkanMemoryAllocationService
    {
    public:
        explicit MemoryAllocationService(NARC_DI_IMPORT_COMPONENT(VulkanInstance), NARC_DI_IMPORT_COMPONENT(VulkanDevice));
        ~MemoryAllocationService() override;

        NARC_QUERY_OVERRIDE(bool, deallocBuffer, VkBuffer& buffer, VmaAllocation& alloc);
        NARC_QUERY_OVERRIDE(VulkanServiceQuery<VmaAllocationInfo>, allocBuffer, const VkBufferCreateInfo& infos, VkBuffer& buffer, VmaAllocation& alloc);
        
        NARC_CMD_OVERRIDE(mapMemory, const void* data, const VkDeviceSize& dataSize, const VmaAllocation& alloc);

    private:
        narc_core::injected_component<VulkanInstance> m_instance;
        narc_core::injected_component<VulkanDevice> m_device;

        mutable VmaAllocator m_allocator = VK_NULL_HANDLE;
        
        mutable size_t m_allocatedItemCount = 0;
        
        void checkBeforeAllocItem() const;
        void checkAfterDeallocItem() const;
        
        void createVmaAllocator() const;
        void destroyVmaAllocator() const;
    };
} // narc_engine
