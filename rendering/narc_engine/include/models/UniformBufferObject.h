#pragma once

#include <NarcMath.h>

namespace narc_engine
{
    struct UniformBufferObject
    {
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Proj;
    };
}
