//
// Created by theo on 12/15/25.
//

#pragma once

#include <vk_mem_alloc.h>

namespace narc_engine {
    class VulkanVertexBuffer
    {
    public:
        explicit VulkanVertexBuffer(NARC_DI_IMPORT_COMPONENT(IVulkanMemoryAllocationService));
        ~VulkanVertexBuffer();


        void setData(const void* data, size_t size);

        NARC_GETTER(VkBuffer, getHandle, m_vertexBuffer);

    private:
        narc_core::injected_component<IVulkanMemoryAllocationService> m_allocator;

        VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
    };
} // narc_engine
