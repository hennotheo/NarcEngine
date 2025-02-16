#pragma once

#include <vulkan/vulkan.h>

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

        VkDevice m_linkedDevice;

        void init();
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    };
}
