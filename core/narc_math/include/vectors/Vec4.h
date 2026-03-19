//
// Created by theo on 3/16/26.
//

#pragma once
#include "matrices/Matrix4.h"

namespace narc_math {
    struct alignas(4) NARC_ENGINE_API Vec4
    {
        union
        {
            struct
            {
                float X, Y, Z, W;
            };

            float Data[4] = {0, 0, 0, 0};
        };

        Vec4();
        Vec4(const Vec4& vector);
        Vec4(float x, float y, float z, float w);

        ~Vec4();

        // bool operator==(const Vec4& other) const;
        // bool operator!=(const Vec4& other) const { return !(*this == other); }
        // Vec4 operator+(const Vec4& other) const;
        // Vec4 operator-(const Vec4& other) const;
        Vec4 operator*(float scalar) const;
        // Vec4& operator+=(const Vec4& other);
        // Vec4& operator-=(const Vec4& other);
        Vec4& operator*=(float scalar);

        // NO_DISCARD float dot(const Vec4& other) const;
        // NO_DISCARD Vec4 cross(const Vec4& other) const;
        // NO_DISCARD float length() const;
        // NO_DISCARD Vec4 normalized() const;
        // NO_DISCARD float distance(const Vec4& other) const;
    };
}
