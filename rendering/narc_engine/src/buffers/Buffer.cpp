#include "buffers/Buffer.h"

#include <NarcLog.h>
#include "Engine.h"

namespace narc_engine
{
    Buffer::Buffer() = default;

    Buffer::~Buffer()
    {
        Buffer::release();
    }

    void Buffer::release()
    {
        vkDestroyBuffer(getVkDevice(), m_buffer, nullptr);
        vkFreeMemory(getVkDevice(), m_bufferMemory, nullptr);
    }

    void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                              VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(getVkDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(getVkDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = getDeviceHandler()->getPhysicalDevice()->findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(getVkDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(getVkDevice(), buffer, bufferMemory, 0);
    }
}
