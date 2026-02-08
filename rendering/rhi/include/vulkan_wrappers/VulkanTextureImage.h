//
// Created by theo on 12/22/25.
//

#pragma once

namespace narc_engine {
    class VulkanCommandBuffer;
    class VulkanCommandPool;

    class VulkanTextureImage : narc_core::IInitialisable
    {
    public:
        explicit VulkanTextureImage(NARC_DI_IMPORT_SERVICE(IVulkanMemoryAllocationService),
                                    NARC_DI_IMPORT_COMPONENT(VulkanCommandPool),
                                    NARC_DI_IMPORT_SERVICE(ICmdService));

        ~VulkanTextureImage() override;

        NARC_IMPL_INITIALISABLE();

        NARC_SETTER(std::string_view, path, m_path);

        NARC_GETTER(uint32_t, getSize, m_allocationInfo.size);

    private:
        narc_core::injected_service<IVulkanMemoryAllocationService> m_allocator;
        narc_core::injected_component<VulkanCommandPool> m_commandPool;
        narc_core::injected_component<ICmdService> m_cmdService;

        std::string m_path;

        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        VmaAllocationInfo m_allocationInfo{};

        void transitionImageLayout(const VulkanCommandBuffer& cmd, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask,
                                   uint32_t
                                   mipLevels);
    };
} // narc_engine
