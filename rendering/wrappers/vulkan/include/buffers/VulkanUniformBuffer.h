//
// Created by theo on 12/19/25.
//

#pragma once

namespace narc_engine {
    class VulkanUniformBuffer  : public IVulkanBuffer {
    public:
        explicit VulkanUniformBuffer(NARC_DI_IMPORT_COMPONENT(IVulkanMemoryAllocationService));
        ~VulkanUniformBuffer() override;
        
        NARC_OVERRIDE_GETTER(const VkBuffer&, getHandle, m_buffer);
        
        void allocate(const VkDeviceSize& size);
        void deallocate();

        void setData(const VkDeviceSize& size, const VulkanMemory* data);

    private:
        narc_core::injected_component<IVulkanMemoryAllocationService> m_allocator;
        
        VkBuffer m_buffer;
        VmaAllocation m_allocation;
    };
} // narc_engine
