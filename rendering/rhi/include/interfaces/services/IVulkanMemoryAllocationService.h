//
// Created by theo on 12/15/25.
//

#pragma once

#include <vk_mem_alloc.h>

namespace narc_engine {
    class IVulkanMemoryAllocationService : public IService
    {
    public:
        ~IVulkanMemoryAllocationService() override = default;

        NARC_PURE_VIRTUAL_QUERY(bool, deallocBuffer, VkBuffer& buffer, VmaAllocation& alloc);
        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VmaAllocationInfo>, allocBuffer, const VkBufferCreateInfo& infos, VkBuffer& buffer, VmaAllocation& alloc);
        
        NARC_PURE_VIRTUAL_CMD(mapMemory, const void* data, const VkDeviceSize& dataSize, const VmaAllocation& alloc);
    };
}
