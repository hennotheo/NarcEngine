#include "buffers/UniformBuffer.h"

namespace narc_engine
{
    UniformBuffer::UniformBuffer(const VkDeviceSize& bufferSize)
    {
        this->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_buffer, this->m_bufferMemory);

        vkMapMemory(getVkDevice(), this->m_bufferMemory, 0, bufferSize, 0, &uniformBuffersMapped);
    }

    void UniformBuffer::setData(const UniformBufferObject& ubo)
    {
        memcpy(uniformBuffersMapped, &ubo, sizeof(ubo));
    }
}
