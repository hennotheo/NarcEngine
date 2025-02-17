#include "include/buffers/StaggingBuffer.h"

#include "include/Engine.h"

namespace narc_engine
{    
    void StaggingBuffer::create(VkDeviceSize bufferSize)
    {
        this->init();

        m_bufferSize = bufferSize;
        this->createBuffer(m_bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_buffer, this->m_bufferMemory);
    }

    void StaggingBuffer::input(const void* input)
    {
        void* data;
        vkMapMemory(this->m_linkedDevice->getDevice(), this->m_bufferMemory, 0, m_bufferSize, 0, &data);
        memcpy(data, input, (size_t)m_bufferSize);
        vkUnmapMemory(this->m_linkedDevice->getDevice(), this->m_bufferMemory);
    }
}
