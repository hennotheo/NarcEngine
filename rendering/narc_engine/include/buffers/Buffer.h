#pragma once

#include <vulkan/vulkan.h>

#include "core/devices/DeviceHandler.h"

#include "platform/vulkan/DeviceMemory.h"

namespace narc_engine
{
    class Buffer : public narc_core::IGetter<VkBuffer>
    {

    public:
        Buffer(VkBufferUsageFlags usage);
        virtual ~Buffer();

        NARC_IMPL_IGETTER(VkBuffer, m_buffer);
        VkBuffer getBuffer() const { return m_buffer; }

    protected:
        VkBuffer m_buffer;
        DeviceMemory m_bufferMemory;
        VkBufferUsageFlags m_usage;

        void createBuffer(VkDeviceSize size, VkMemoryPropertyFlags properties, VkBuffer& buffer, DeviceMemory& bufferMemory);
        virtual void release();
    };
}