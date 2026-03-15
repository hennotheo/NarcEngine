//
// Created by theo on 3/15/26.
//


#pragma once

namespace narc_math {
    struct alignas(4) NARC_ENGINE_API Vec2Int
    {
        int32_t Data[2] = {0, 0};

        Vec2Int();
        Vec2Int(const Vec2Int& vector);
        Vec2Int(int32_t x, int32_t y);

        ~Vec2Int();

        NARC_GETTER(int32_t, X, Data[0])
        NARC_GETTER(int32_t, Y, Data[1])

        NARC_SETTER(int32_t, setX, Data[0])
        NARC_SETTER(int32_t, setY, Data[1])
    };
}
