#include "buffers/StagingBuffer.h"

#include "Engine.h"

namespace narc_engine
{    
    void StagingBuffer::create(VkDeviceSize bufferSize)
    {
        m_bufferSize = bufferSize;
        NARCLOG_DEBUG("Stagging");
        this->createBuffer(
            m_bufferSize, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            this->m_buffer, 
            this->m_bufferMemory);
    }

    void StagingBuffer::input(const void* input)
    {
        void* data;
        m_bufferMemory.map(0, m_bufferSize, &data);
        m_bufferMemory.copy(data, input, m_bufferSize);
        m_bufferMemory.unmap();
    }
}