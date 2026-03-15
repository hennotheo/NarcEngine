//
// Created by theo on 3/16/26.
//

#pragma once

namespace narc_math {
    struct alignas(4) NARC_ENGINE_API Vec4
    {
        float Data[4] = {0, 0, 0};

        Vec4();
        Vec4(const Vec4& vector);
        Vec4(float x, float y, float z, float w);

        ~Vec4();

        NARC_GETTER(float, X, Data[0])
        NARC_GETTER(float, Y, Data[1])
        NARC_GETTER(float, Z, Data[2])
        NARC_GETTER(float, W, Data[3])

        NARC_SETTER(float, setX, Data[0])
        NARC_SETTER(float, setY, Data[1])
        NARC_SETTER(float, setZ, Data[2])
        NARC_SETTER(float, setW, Data[3])

        // bool operator==(const Vec4& other) const;
        // bool operator!=(const Vec4& other) const { return !(*this == other); }
        // Vec4 operator+(const Vec4& other) const;
        // Vec4 operator-(const Vec4& other) const;
        Vec4 operator*(float scalar) const;
        Vec4 operator*(Vec4 vector) const;
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
