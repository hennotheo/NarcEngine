//
// Created by theo on 3/16/26.
//

#include "vectors/Vec2Int.h"

namespace narc_math {

    Vec2Int::Vec2Int() = default;

    Vec2Int::Vec2Int(const Vec2Int& vector)
    {
        Data[0] = vector.Data[0];
        Data[1] = vector.Data[1];
    }

    Vec2Int::Vec2Int(const int32_t x, const int32_t y)
    {
        Data[0] = x;
        Data[1] = y;
    }

    Vec2Int::~Vec2Int() = default;
}
