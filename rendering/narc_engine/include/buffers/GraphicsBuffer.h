#pragma once
#include "Buffer.h"

namespace narc_engine
{
    template <typename T>
    class GraphicsBuffer final : public Buffer
    {
    public:
        GraphicsBuffer(): Buffer()
        {
        }

        void create(const std::vector<T>& input, VkBufferUsageFlagBits usage);
    };
}