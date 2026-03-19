//
// Created by theoh on 3/4/2025.
//

#pragma once

#include "pch.h"

#include "matrices/Matrix4.h"
#include "vectors/Vec3.h"
#include "vectors/Vec4.h"
#include "vectors/Vec2.h"
#include "vectors/Vec2Int.h"

#include "Quaternion.h"

#include "Extent.h"

#define NARC_KB_SIZEOF(obj) (static_cast<double>(sizeof(obj)) / 1024.0)

namespace narc_math {
    constexpr float EPSILON_F = 1e-6f;
    constexpr float PI_F = 3.14159265358979323846f;
}
