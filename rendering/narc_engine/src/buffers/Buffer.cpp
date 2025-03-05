#include "buffers/Buffer.h"

#include <NarcLog.h>
#include "Engine.h"

namespace narc_engine
{
    Buffer::Buffer()
    {
        m_linkedDevice = Engine::getInstance()->getDevice();
        if (!m_linkedDevice)
        {
            NARCLOG_FATAL("Failed to get linked device from Engine instance.");
        }
    }

    void Buffer::release()
    {
        vkDestroyBuffer(m_linkedDevice->getDevice(), m_buffer, nullptr);
        vkFreeMemory(m_linkedDevice->getDevice(), m_bufferMemory, nullptr);
    }

    void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                              VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_linkedDevice->getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_linkedDevice->getDevice(), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = m_linkedDevice->findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_linkedDevice->getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(m_linkedDevice->getDevice(), buffer, bufferMemory, 0);
    }
}
