#include "include/buffers/Buffer.h"

#include <stdexcept>
#include <vector>

#include "include/Engine.h"

namespace narc_engine
{
    void Buffer::release()
    {
        vkDestroyBuffer(m_linkedDevice, m_buffer, nullptr);
        vkFreeMemory(m_linkedDevice, m_bufferMemory, nullptr);
    }
    
    void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_linkedDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(m_linkedDevice, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = Engine::getInstance()->findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_linkedDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(m_linkedDevice, buffer, bufferMemory, 0);
    }
}
