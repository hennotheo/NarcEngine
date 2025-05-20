#pragma once

#include <NarcMath.h>

namespace narc_engine
{
    constexpr uint16_t UNIFORM_BUFFER_OBJECT_MAX_INSTANCES = 10;

    struct UniformBufferObject
    {
        glm::mat4 Model[UNIFORM_BUFFER_OBJECT_MAX_INSTANCES];
        glm::mat4 View;
        glm::mat4 Proj;
    };
}
