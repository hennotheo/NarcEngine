#include "buffers/Buffer.h"

#include "Engine.h"

namespace narc_engine
{
    Buffer::Buffer(VkBufferUsageFlags usage) : DeviceComponent(), m_usage(usage)
    {
        m_bufferMemory = DeviceMemory();
    };

    Buffer::~Buffer()
    {
        Buffer::release();
    }

    void Buffer::release()
    {
        vkDestroyBuffer(getVkDevice(), m_buffer, nullptr);
        m_bufferMemory.release();
    }

    void Buffer::createBuffer(VkDeviceSize size, VkMemoryPropertyFlags properties, VkBuffer& buffer, DeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = m_usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(getVkDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(getVkDevice(), buffer, &memRequirements);

        bufferMemory.setSize(memRequirements.size);
        bufferMemory.setMemoryTypeIndex(getDeviceHandler()->getPhysicalDevice()->findMemoryType(memRequirements.memoryTypeBits, properties));
        bufferMemory.allocate();

        vkBindBufferMemory(getVkDevice(), buffer, bufferMemory.get(), 0);
    }
}
