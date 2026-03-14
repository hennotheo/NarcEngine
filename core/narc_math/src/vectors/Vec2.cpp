//
// Created by theo on 3/14/26.
//

#include "vectors/Vec2.h"

namespace narc_math {

    Vec2::Vec2()
    {
        Data[0] = 0.0f;
        Data[1] = 0.0f;
    }

    Vec2::Vec2(const Vec2& vector)
    {
        Data[0] = vector.Data[0];
        Data[1] = vector.Data[1];
    }

    Vec2::Vec2(float x, float y)
    {
        Data[0] = x;
        Data[1] = y;
    }

    Vec2::~Vec2() = default;

    bool Vec2::operator==(const Vec2& other) const
    {
        constexpr float epsilon = 1e-6f;

        return std::fabs(Data[0] - other.Data[0]) < epsilon &&
               std::fabs(Data[1] - other.Data[1]) < epsilon;
    }

    Vec2 Vec2::operator+(const Vec2& other) const
    {
        return Vec2{
                Data[0] + other.Data[0],
                Data[1] + other.Data[1]
        };
    }

    Vec2 Vec2::operator-(const Vec2& other) const
    {
        return Vec2{
                Data[0] - other.Data[0],
                Data[1] - other.Data[1]
        };
    }

    Vec2 Vec2::operator*(float scalar) const
    {
        return Vec2{
                Data[0] * scalar,
                Data[1] * scalar
        };
    }

    Vec2& Vec2::operator+=(const Vec2& other)
    {
        Data[0] += other.Data[0];
        Data[1] += other.Data[1];
        return *this;
    }

    Vec2& Vec2::operator-=(const Vec2& other)
    {
        Data[0] -= other.Data[0];
        Data[1] -= other.Data[1];
        return *this;
    }

    Vec2& Vec2::operator*=(float scalar)
    {
        Data[0] *= scalar;
        Data[1] *= scalar;
        return *this;
    }

    float Vec2::dot(const Vec2& other) const
    {
        return Data[0] * other.Data[0] +
               Data[1] * other.Data[1];
    }

    float Vec2::cross(const Vec2& other) const
    {
        return Data[0] * other.Data[1] -
               Data[1] * other.Data[0];
    }


    float Vec2::length() const
    {
        return std::sqrt(dot(*this));
    }

    Vec2 Vec2::normalized() const
    {
        const float len = length();

        if (len < 1e-6f)
        {
            return Vec2{0.0f, 0.0f};
        }

        return Vec2{
                Data[0] / len,
                Data[1] / len
        };
    }

    float Vec2::distance(const Vec2& other) const
    {
        return (*this - other).length();
    }

    Vec2 Vec2::perpendicular() const
    {
        return Vec2{-Data[1], Data[0]};
    }

    Vec2 Vec2::reflect(const Vec2& normal) const
    {
        return *this - normal * (2.0f * dot(normal));
    }

    Vec2 Vec2::rotate(float r) const
    {
        float c = std::cos(r);
        float s = std::sin(r);

        return Vec2{
            Data[0] * c - Data[1] * s,
            Data[0] * s + Data[1] * c
        };
    }

    Vec2 Vec2::lerp(const Vec2& a, const Vec2& b, float t)
    {
        return a + (b - a) * t;
    }

    float Vec2::lengthSquared() const
    {
        return dot(*this);
    }
}
