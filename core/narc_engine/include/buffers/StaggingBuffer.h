#pragma once

#include "Buffer.h"

namespace narc_engine
{
    class StaggingBuffer : public Buffer
    {
    public:        
        void create(VkDeviceSize bufferSize);
        void input(const void* input);

    private:
        VkDeviceSize m_bufferSize;
    };
}
