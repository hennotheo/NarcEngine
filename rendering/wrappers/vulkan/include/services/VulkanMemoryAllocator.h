//
// Created by theo on 12/15/25.
//

#pragma once

namespace narc_engine {
    class VulkanDevice;
    class VulkanInstance;

    class VulkanMemoryAllocator : public narc_core::IInitialisable
    {
    public:
        explicit VulkanMemoryAllocator(const VulkanInstance* instance, const VulkanDevice* device);
        ~VulkanMemoryAllocator() override;

        NARC_IMPL_INITIALISABLE();

        NARC_QUERY(VulkanServiceQuery<VmaAllocationInfo>, allocBuffer, const VkBufferCreateInfo& infos, VkBuffer& buffer, VmaAllocation& alloc);
        NARC_QUERY(bool, deallocBuffer, VkBuffer& buffer, VmaAllocation& alloc);
        
        NARC_QUERY(VulkanServiceQuery<VmaAllocationInfo>, allocImage, const VkImageCreateInfo& infos, VkImage& buffer, VmaAllocation& alloc);
        NARC_QUERY(bool, deallocImage, VkImage& image, VmaAllocation& alloc);

        NARC_QUERY(VulkanServiceQuery<VkImageView>, allocImageView, const VkImageViewCreateInfo& infos);
        NARC_QUERY(bool, deallocImageView, const VkImageView& imageView);

        NARC_QUERY(VulkanServiceQuery<VkSampler>, allocSampler, VkSamplerCreateInfo& infos);
        NARC_QUERY(bool, deallocSampler, const VkSampler& sampler);

        narc_core::result mapMemory(const Memory* data, const VkDeviceSize& dataSize, const VmaAllocation& alloc) const noexcept;

    private:
        const VulkanInstance* m_instance;
        const VulkanDevice* m_device;

        VmaAllocator m_allocator = VK_NULL_HANDLE;
        size_t m_allocatedItemCount = 0;
        
        void createVmaAllocator();
        void destroyVmaAllocator();
    };
} // narc_engine
