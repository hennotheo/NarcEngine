#include "buffers/UniformBuffer.h"

namespace narc_engine
{
    UniformBuffer::UniformBuffer() : Buffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
    {
        m_minBufferObjectSize = NARC_PHYSICAL_DEVICE->getPhysicalDeviceProperties().limits.minUniformBufferOffsetAlignment;
        m_size = m_minBufferObjectSize;
        this->createBuffer(m_size, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_buffer, this->m_bufferMemory);
    }

    void UniformBuffer::beginRegister(VkDeviceSize totalSize)
    {
        if (totalSize > m_size)
        {
            grow(totalSize);
        }

        m_bufferMemory.map(0, m_size, &m_uniformBuffersMapped);
        m_bufferSizes.clear();
        m_currentRegisteredSize = 0;
    }

    void UniformBuffer::registerBufferObject(void* data, VkDeviceSize size)
    {
        uint32_t objectSize = getValidUniformBufferSize(size);
        m_bufferSizes.push_back(objectSize);

        memcpy(static_cast<char*>(m_uniformBuffersMapped) + m_currentRegisteredSize, data, size);
        m_currentRegisteredSize += objectSize;
    }

    void UniformBuffer::endRegister()
    {
        m_bufferMemory.unmap();
    }

    void UniformBuffer::grow(VkDeviceSize newSize)
    {
        while (newSize > m_size)
        {
            m_size *= 2;
        }

        this->recreate();
    }

    void UniformBuffer::recreate()
    {
        release();
        this->createBuffer(m_size, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_buffer, this->m_bufferMemory);
    }

    const VkDeviceSize UniformBuffer::getValidUniformBufferSize(VkDeviceSize size) const
    {
        return ((size + m_minBufferObjectSize - 1) / m_minBufferObjectSize) * m_minBufferObjectSize;
    }
}
