//
// Created by theo on 3/14/26.
//


#pragma once

namespace narc_math {
    struct alignas(4) NARC_ENGINE_API Vec2
    {
        float Data[2] = {0, 0};

        Vec2();
        Vec2(const Vec2& vector);
        Vec2(float x, float y);

        ~Vec2();

        NARC_GETTER(float, X, Data[0])
        NARC_GETTER(float, Y, Data[1])

        NARC_SETTER(float, setX, Data[0])
        NARC_SETTER(float, setY, Data[1])

        bool operator==(const Vec2& other) const;
        bool operator!=(const Vec2& other) const { return !(*this == other); }
        Vec2 operator+(const Vec2& other) const;
        Vec2 operator-(const Vec2& other) const;
        Vec2 operator*(float scalar) const;
        Vec2& operator+=(const Vec2& other);
        Vec2& operator-=(const Vec2& other);
        Vec2& operator*=(float scalar);

        NO_DISCARD float dot(const Vec2& other) const;
        NO_DISCARD float cross(const Vec2& other) const;
        NO_DISCARD float length() const;
        NO_DISCARD Vec2 normalized() const;
        NO_DISCARD float distance(const Vec2& other) const;

        Vec2 perpendicular() const;
        Vec2 reflect(const Vec2& normal) const;
        Vec2 rotate(float radians) const;

        static Vec2 lerp(const Vec2& a, const Vec2& b, float t);
        float lengthSquared() const;
    };
}