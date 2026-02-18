//
// Created by theo on 12/15/25.
//

#pragma once

namespace narc_engine {
    struct Vertex
    {
        narc_math::Vec2 pos;
        narc_math::Vec3 color;
        narc_math::Vec2 texCoord;
    };

    const std::vector<Vertex> s_vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> s_indices = {
            0, 1, 2, 2, 3, 0
    };
}
