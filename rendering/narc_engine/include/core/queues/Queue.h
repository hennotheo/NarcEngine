#pragma once 

#include <vulkan/vulkan_core.h>

namespace narc_engine
{
    class Queue
    {
    public:
        virtual ~Queue() = default;

    protected:
        VkQueue m_queue;
    };
}