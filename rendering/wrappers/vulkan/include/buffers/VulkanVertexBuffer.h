//
// Created by theo on 12/15/25.
//

#pragma once

#include <vk_mem_alloc.h>

namespace narc_engine {    
    class VulkanVertexBuffer : public IVulkanBuffer
    {
    public:
        explicit VulkanVertexBuffer(NARC_DI_IMPORT_COMPONENT(IVulkanMemoryAllocationService));
        ~VulkanVertexBuffer() override;

        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_vertexBuffer);

    private:
        narc_core::injected_component<IVulkanMemoryAllocationService> m_allocator;

        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
    };
} // narc_engine
