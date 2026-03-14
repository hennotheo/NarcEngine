//
// Created by theoh on 3/4/2025.
//

#pragma once

#include "pch.h"

//TODO: TEMP
#include "../src/GlmCore.h"

#include "matrices/Matrix4.h"
#include "vectors/Vec3.h"
#include "vectors/Vec2.h"

#define NARC_KB_SIZEOF(obj) (static_cast<double>(sizeof(obj)) / 1024.0)

namespace narc_math {
    constexpr float EPSILON_F = 1e-6f;

    using Vec2Int = glm::ivec2;

    using Vec3Int = glm::ivec3;
}

#include "Transform.h"
#include "Extent.h"
