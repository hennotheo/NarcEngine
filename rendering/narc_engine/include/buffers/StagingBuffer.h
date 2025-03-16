#pragma once

#include "Buffer.h"

namespace narc_engine
{
    class StagingBuffer final : public Buffer
    {
    public:
        StagingBuffer() : Buffer(){}

        void create(VkDeviceSize bufferSize);
        void input(const void* input);

    private:
        VkDeviceSize m_bufferSize;
    };
}
