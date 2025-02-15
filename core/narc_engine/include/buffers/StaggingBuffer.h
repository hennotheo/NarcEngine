#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

#include "Buffer.h"

namespace narc_engine
{
    template <class T>
    class StaggingBuffer : public Buffer
    {
    public:        
        void create(const std::vector<T>& input, VkBufferUsageFlagBits usage);
    };
}
