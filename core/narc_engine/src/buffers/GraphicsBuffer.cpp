#include "buffers/GraphicsBuffer.h"

#include "Engine.h"
#include "data/Vertex.h"
#include "buffers/StaggingBuffer.h"

namespace narc_engine
{
    template class GraphicsBuffer<Vertex>;
    template class GraphicsBuffer<uint16_t>;

    template <typename T>
    void GraphicsBuffer<T>::create(const std::vector<T>& input, VkBufferUsageFlagBits usage)//vertex : VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, index : VK_BUFFER_USAGE_INDEX_BUFFER_BIT 
    {
        this->init();
        
        VkDeviceSize bufferSize = sizeof(input[0]) * input.size();

        StaggingBuffer stagingBuffer;
        stagingBuffer.create(bufferSize);
        stagingBuffer.input(input.data());

        this->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->m_buffer, this->m_bufferMemory);

        Engine::getInstance()->copyBuffer(stagingBuffer.getBuffer(), this->m_buffer, bufferSize);
        stagingBuffer.release();
    }
}
