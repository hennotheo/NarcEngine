#pragma once 

#include <vulkan/vulkan_core.h>

namespace narc_engine
{
    class Queue
    {
    public:
        virtual ~Queue() = default;

        NARC_GETTER(const VkQueue&, getVkQueue, m_queue);

    protected:
        VkQueue m_queue;
    };
}