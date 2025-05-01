#pragma once 

#include <vulkan/vulkan_core.h>

namespace narc_engine
{
    class Queue
    {
    public:
        virtual ~Queue() = default;

        GETTER const VkQueue& getVkQueue() const { return m_queue; }

    protected:
        VkQueue m_queue;
    };
}