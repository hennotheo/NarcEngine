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

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VmaAllocationInfo>, allocBuffer, const VkBufferCreateInfo& infos, VkBuffer& buffer, VmaAllocation& alloc);
        NARC_PURE_VIRTUAL_QUERY(bool, deallocBuffer, VkBuffer& buffer, VmaAllocation& alloc);
        
        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VmaAllocationInfo>, allocImage, const VkImageCreateInfo& infos, VkImage& buffer, VmaAllocation& alloc);
        NARC_PURE_VIRTUAL_QUERY(bool, deallocImage, VkImage& buffer, VmaAllocation& alloc);

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VkSampler>, allocSampler, VkSamplerCreateInfo& infos);
        NARC_PURE_VIRTUAL_QUERY(bool, deallocSampler, const VkSampler& sampler);

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VkImageView>, allocImageView, const VkImageViewCreateInfo& infos);
        NARC_PURE_VIRTUAL_QUERY(bool, deallocImageView, const VkImageView& imageView);

        NARC_PURE_VIRTUAL_CMD(mapMemory, const VulkanMemory* data, const VkDeviceSize& dataSize, const VmaAllocation& alloc);
    };
}
