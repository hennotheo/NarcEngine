#pragma once

#include <vulkan/vulkan.h>
#include "core/DeviceHandler.h"

namespace narc_engine
{
    class Buffer
    {
    public:
        Buffer();
        virtual ~Buffer();

        VkBuffer getBuffer() const { return m_buffer; }

    protected:
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

        const DeviceHandler* m_linkedDevice;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        virtual void release();
    };
}