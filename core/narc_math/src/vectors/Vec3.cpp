//
// Created by theo on 3/12/26.
//

#include "vectors/Vec3.h"

#include <glm/vec3.hpp>

#include "NarcMath.h"

namespace narc_math {

    Vec3::Vec3() = default;

    Vec3::Vec3(const Vec3& vector)
    {
        Data[0] = vector.Data[0];
        Data[1] = vector.Data[1];
        Data[2] = vector.Data[2];
    }

    Vec3::Vec3(const float x, const float y, const float z)
    {
        Data[0] = x;
        Data[1] = y;
        Data[2] = z;
    }

    Vec3::~Vec3() = default;

    bool Vec3::operator==(const Vec3& other) const
    {
        return std::fabs(Data[0] - other.Data[0]) < EPSILON_F &&
               std::fabs(Data[1] - other.Data[1]) < EPSILON_F &&
               std::fabs(Data[2] - other.Data[2]) < EPSILON_F;
    }

    Vec3 Vec3::operator+(const Vec3& other) const
    {
        return Vec3{
                Data[0] + other.Data[0],
                Data[1] + other.Data[1],
                Data[2] + other.Data[2]
        };
    }

    Vec3 Vec3::operator-(const Vec3& other) const
    {
        return Vec3{
                Data[0] - other.Data[0],
                Data[1] - other.Data[1],
                Data[2] - other.Data[2]
        };
    }

    Vec3 Vec3::operator*(const float scalar) const
    {
        return Vec3{
                Data[0] * scalar,
                Data[1] * scalar,
                Data[2] * scalar
        };
    }

    Vec3& Vec3::operator+=(const Vec3& other)
    {
        Data[0] += other.Data[0];
        Data[1] += other.Data[1];
        Data[2] += other.Data[2];

        return *this;
    }

    Vec3& Vec3::operator-=(const Vec3& other)
    {
        Data[0] -= other.Data[0];
        Data[1] -= other.Data[1];
        Data[2] -= other.Data[2];

        return *this;
    }

    Vec3& Vec3::operator*=(const float scalar)
    {
        Data[0] *= scalar;
        Data[1] *= scalar;
        Data[2] *= scalar;

        return *this;
    }

    float Vec3::dot(const Vec3& other) const
    {
        return Data[0] * other.Data[0] + Data[1] * other.Data[1] + Data[2] * other.Data[2];
    }

    Vec3 Vec3::cross(const Vec3& other) const
    {
        return {
                Data[1] * other.Data[2] - Data[2] * other.Data[1],
                Data[2] * other.Data[0] - Data[0] * other.Data[2],
                Data[0] * other.Data[1] - Data[1] * other.Data[0]
        };
    }

    float Vec3::length() const
    {
        return std::sqrt(dot(*this));
    }

    Vec3 Vec3::normalized() const
    {
        const float len = length();

        return len < EPSILON_F
                   ? Vec3{}
                   : *this * (1.0f / len);
    }

    float Vec3::distance(const Vec3& other) const
    {
        return (*this - other).length();
    }
}
