#pragma once
#include "Buffer.h"

namespace narc_engine
{
    class UniformBuffer : public Buffer
    {
    public:
        void create(VkDeviceSize bufferSize);

    private:
        void* uniformBuffersMapped;
    };
}
