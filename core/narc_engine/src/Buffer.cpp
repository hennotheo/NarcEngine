#include "include/Buffer.h"

#include <stdexcept>
#include <vector>

#include "include/Engine.h"

namespace narc_engine
{
    template class Buffer<Vertex>;
    template class Buffer<uint16_t>;
    
    template <typename T>
    void Buffer<T>::create(const std::vector<T>& input, VkBufferUsageFlagBits usage)//vertex : VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, index : VK_BUFFER_USAGE_INDEX_BUFFER_BIT 
    {
        m_linkedDevice = Engine::getInstance()->getDevice();
        
        VkDeviceSize bufferSize = sizeof(input[0]) * input.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(m_linkedDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, input.data(), (size_t)bufferSize);
        vkUnmapMemory(m_linkedDevice, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory);

        Engine::getInstance()->copyBuffer(stagingBuffer, m_buffer, bufferSize);
        vkDestroyBuffer(m_linkedDevice, stagingBuffer, nullptr);
        vkFreeMemory(m_linkedDevice, stagingBufferMemory, nullptr);
    }

    template <class T>
    void Buffer<T>::release()
    {
        vkDestroyBuffer(m_linkedDevice, m_buffer, nullptr);
        vkFreeMemory(m_linkedDevice, m_bufferMemory, nullptr);
    }

    template <class T>
    void Buffer<T>::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
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
