#pragma once

#include "Queue.h"

namespace narc_engine
{
    class EngineBuilder;

    class GraphicsQueue final : public Queue
    {
    public:
        explicit GraphicsQueue(const EngineBuilder* builder);
        GraphicsQueue(const GraphicsQueue&) = delete;
        ~GraphicsQueue() override = default;

        void waitIdle() const;
        VkResult submit(uint32_t submitCount, const VkSubmitInfo *submitInfo, VkFence fence) const;
    };
} // namespace narc_engine
