//
// Created by theo on 12/22/25.
//

#pragma once

namespace narc_engine {
    class VulkanMemoryAllocator;
    class VulkanCommandBuffer;
    class VulkanCommandPool;

    class VulkanTextureImage : narc_core::IInitialisable
    {
    public:
        explicit VulkanTextureImage(const VulkanMemoryAllocator* allocator);

        ~VulkanTextureImage() override;

        NARC_IMPL_INITIALISABLE();

        NARC_SETTER(std::string_view, path, m_path);

        NARC_GETTER(uint32_t, getSize, m_allocationInfo.size);
        NARC_GETTER(VkImageView, getView, m_view);
        NARC_GETTER(VkSampler, getSampler, m_sampler);

    private:
        const VulkanMemoryAllocator* m_allocator;

        std::string m_path;

        VkImage m_image = VK_NULL_HANDLE;
        VkImageView m_view = VK_NULL_HANDLE;
        VkSampler m_sampler = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo{};

        void transitionImageLayout(const VulkanCommandBuffer& cmd, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask,
                                   uint32_t
                                   mipLevels);

        void createImageViewCreateInfo();
    };
} // narc_engine
