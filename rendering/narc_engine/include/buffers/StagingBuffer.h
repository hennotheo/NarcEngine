#pragma once

#include "Buffer.h"

namespace narc_engine
{
    class StagingBuffer final : public Buffer
    {
    public:
        StagingBuffer() : Buffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT){}

        void create(VkDeviceSize bufferSize);
        void input(const void* input);

    private:
        VkDeviceSize m_bufferSize;
    };
}
