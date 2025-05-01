#pragma once

#include "Queue.h"

namespace narc_engine
{
    class EngineBuilder;

    class PresentQueue final : public Queue
    {
    public:
        explicit PresentQueue(const EngineBuilder* builder);
        PresentQueue(const PresentQueue&) = delete;
        ~PresentQueue() override = default;

        VkResult presentKHR(const VkPresentInfoKHR* presentInfo) const;
    };
} // namespace narc_engine