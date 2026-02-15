//
// Created by theo on 12/17/25.
//

#pragma once

namespace narc_engine {
    class VulkanIndexBuffer : public IVulkanBuffer
    {
    public:
        explicit VulkanIndexBuffer(NARC_DI_IMPORT_COMPONENT(IVulkanMemoryAllocationService));
        ~VulkanIndexBuffer() override;

        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_vertexBuffer);

    private:
        narc_core::injected_component<IVulkanMemoryAllocationService> m_allocator;

        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
    };
} // narc_engine
