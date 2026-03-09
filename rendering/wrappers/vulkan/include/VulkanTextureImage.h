//
// Created by theo on 12/22/25.
//

#pragma once

namespace narc_engine {
    class VulkanMemoryAllocator;
    class VulkanCommandBuffer;
    class VulkanCommandPool;

    class VulkanTextureImage : public IImage
    {
    public:
        explicit VulkanTextureImage(const VulkanMemoryAllocator* allocator);

        ~VulkanTextureImage() override;

        NARC_IMPL_INITIALISABLE();

        NARC_SETTER(narc_math::Extent&, setExtent, m_extent);

        NARC_OVERRIDE_GETTER(narc_math::Extent, getExtent, m_extent);
        NARC_GETTER(uint32_t, getSize, m_allocationInfo.size);
        NARC_GETTER(VkImage, getImage, m_image);
        NARC_GETTER(VkImageView, getView, m_view);
        NARC_GETTER(VkSampler, getSampler, m_sampler);

    private:
        const VulkanMemoryAllocator* m_allocator;

        narc_math::Extent m_extent;

        VkImage m_image = VK_NULL_HANDLE;
        VkImageView m_view = VK_NULL_HANDLE;
        VkSampler m_sampler = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo{};

        void createImageView();
        void createSampler();
    };
} // narc_engine
