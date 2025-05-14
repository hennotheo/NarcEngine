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

        void setSize(VkDeviceSize size) { m_allocInfo.allocationSize = size; }
        void setMemoryTypeIndex(uint32_t index) { m_allocInfo.memoryTypeIndex = index; }

        void allocate();
        void release();

    private:
        VkDeviceMemory m_memory;
        VkMemoryAllocateInfo m_allocInfo;

        bool m_allocated;
    };
} // namespace narc_engine