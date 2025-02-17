#pragma once

#include <vulkan/vulkan.h>

#include "include/DeviceHandler.h"

namespace narc_engine
{
    class Buffer
    {
    public:
        VkBuffer getBuffer() const { return m_buffer; }

        virtual void release();

    protected:
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

        const DeviceHandler* m_linkedDevice;

        void init();
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    };
}
