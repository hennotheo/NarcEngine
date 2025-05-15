#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"

namespace narc_engine
{
    class DeviceMemory : public DeviceComponent, public narc_core::IGetter<VkDeviceMemory>
    {
    public:
        DeviceMemory();
        ~DeviceMemory();

        NARC_IMPL_IGETTER(VkDeviceMemory, m_memory);

        GETTER bool isAllocated() const { return m_allocated; }

        void setSize(VkDeviceSize size) { m_allocInfo.allocationSize = size; }
        void setMemoryTypeIndex(uint32_t index) { m_allocInfo.memoryTypeIndex = index; }

        void allocate();
        void release();

        void map(VkDeviceSize offset, VkDeviceSize size, void** data);
        void copy(void* dst, const void* src, VkDeviceSize size);
        void unmap();

    private:
        VkDeviceMemory m_memory;
        VkMemoryAllocateInfo m_allocInfo;

        bool m_allocated;
    };
} // namespace narc_engine