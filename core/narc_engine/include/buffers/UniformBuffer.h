#pragma once
#include "Buffer.h"
#include "data/UniformBufferObject.h"

namespace narc_engine
{
    class UniformBuffer : public Buffer
    {
    public:
        void create(const VkDeviceSize& bufferSize);

        void setData(const UniformBufferObject& ubo);
    private:
        void* uniformBuffersMapped;
    };
}
