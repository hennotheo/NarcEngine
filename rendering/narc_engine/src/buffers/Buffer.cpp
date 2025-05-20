#include "buffers/Buffer.h"

#include "Engine.h"

namespace narc_engine
{
    Buffer::Buffer(VkBufferUsageFlags usage) : m_usage(usage)
    {
        m_bufferMemory = DeviceMemory();
    };

    Buffer::~Buffer()
    {
        Buffer::release();
    }

    void Buffer::release()
    {
        vkDestroyBuffer(NARC_DEVICE_HANDLE, m_buffer, nullptr);
        m_bufferMemory.release();
    }

    void Buffer::createBuffer(VkDeviceSize size, VkMemoryPropertyFlags properties, VkBuffer& buffer, DeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = m_usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(NARC_DEVICE_HANDLE, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(NARC_DEVICE_HANDLE, buffer, &memRequirements);

        bufferMemory.setSize(memRequirements.size);
        bufferMemory.setMemoryTypeIndex(NARC_PHYSICAL_DEVICE->findMemoryType(memRequirements.memoryTypeBits, properties));
        bufferMemory.allocate();

        vkBindBufferMemory(NARC_DEVICE_HANDLE, buffer, bufferMemory.get(), 0);
    }
}
