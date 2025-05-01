#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"
#include "core/devices/DeviceHandler.h"

namespace narc_engine
{
    class Buffer : public DeviceComponent
    {
    public:
        Buffer();
        virtual ~Buffer();

        VkBuffer getBuffer() const { return m_buffer; }

    protected:
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        virtual void release();
    };
}