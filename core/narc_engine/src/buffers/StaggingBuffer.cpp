#include "include/buffers/StaggingBuffer.h"

#include "include/Vertex.h"
#include "include/Engine.h"

namespace narc_engine
{
    template class StaggingBuffer<Vertex>;
    template class StaggingBuffer<uint16_t>;
    
    template <class T>
    void StaggingBuffer<T>::create(const std::vector<T>& input, VkBufferUsageFlagBits usage)//vertex : VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, index : VK_BUFFER_USAGE_INDEX_BUFFER_BIT 
    {
        this->init();
        
        VkDeviceSize bufferSize = sizeof(input[0]) * input.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        this->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(this->m_linkedDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, input.data(), (size_t)bufferSize);
        vkUnmapMemory(this->m_linkedDevice, stagingBufferMemory);

        this->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->m_buffer, this->m_bufferMemory);

        Engine::getInstance()->copyBuffer(stagingBuffer, this->m_buffer, bufferSize);
        vkDestroyBuffer(this->m_linkedDevice, stagingBuffer, nullptr);
        vkFreeMemory(this->m_linkedDevice, stagingBufferMemory, nullptr);
    }
}
