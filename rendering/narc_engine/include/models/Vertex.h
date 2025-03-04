#pragma once

#include <NarcMath.h>

struct VkVertexInputBindingDescription;
struct VkVertexInputAttributeDescription;

namespace narc_engine {
    struct NARC_ENGINE_API Vertex
    {
        glm::vec3 Pos;
        glm::vec3 Color;
        glm::vec2 TexCoord;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
    };
}
