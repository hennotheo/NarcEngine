#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"
#include "core/devices/DeviceHandler.h"

#include "platform/vulkan/DeviceMemory.h"

namespace narc_engine
{
    class Buffer : public DeviceComponent
    {
    public:
        Buffer(VkBufferUsageFlags usage);
        virtual ~Buffer();

        VkBuffer getBuffer() const { return m_buffer; }

    protected:
        VkBuffer m_buffer;
        DeviceMemory m_bufferMemory;
        VkBufferUsageFlags m_usage; 

        void createBuffer(VkDeviceSize size, VkMemoryPropertyFlags properties, VkBuffer& buffer, DeviceMemory& bufferMemory);
        virtual void release();
    };
}