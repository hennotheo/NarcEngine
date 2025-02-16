#include "include/buffers/UniformBuffer.h"

#include "include/UniformBufferObject.h"

namespace narc_engine
{
    void UniformBuffer::create(const VkDeviceSize& bufferSize)
    {
        this->init();
        this->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_buffer, this->m_bufferMemory);

        vkMapMemory(this->m_linkedDevice, this->m_bufferMemory, 0, bufferSize, 0, &uniformBuffersMapped);
    }

    void UniformBuffer::setData(const UniformBufferObject& ubo)
    {
        memcpy(uniformBuffersMapped, &ubo, sizeof(ubo));
    }
}
