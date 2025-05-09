#pragma once
#include "Buffer.h"

namespace narc_engine
{
    template <typename T>
    class GraphicsBuffer final : public Buffer
    {
    public:
        GraphicsBuffer(VkBufferUsageFlagBits usage): Buffer(usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
        {
        }

        void create(const std::vector<T>& input);
    };
}