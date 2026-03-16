//
// Created by theo on 3/12/26.
//

#include "vectors/Vec3.h"

#include <glm/vec3.hpp>

#include "NarcMath.h"
#include "../GlmCore.h"

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
        Vec3 result;

        ToGlm(result) = ToGlm(*this) + ToGlm(other);

        return result;
    }

    Vec3 Vec3::operator-(const Vec3& other) const
    {
        Vec3 result;

        ToGlm(result) = ToGlm(*this) - ToGlm(other);

        return result;
    }

    Vec3 Vec3::operator*(const float scalar) const
    {
        Vec3 result;

        ToGlm(result) = ToGlm(*this) * scalar;

        return result;
    }

    Vec3& Vec3::operator+=(const Vec3& other)
    {
        ToGlm(*this) = ToGlm(*this) + ToGlm(other);

        return *this;
    }

    Vec3& Vec3::operator-=(const Vec3& other)
    {
        ToGlm(*this) = ToGlm(*this) - ToGlm(other);

        return *this;
    }

    Vec3& Vec3::operator*=(const float scalar)
    {
        ToGlm(*this) = ToGlm(*this) * scalar;

        return *this;
    }

    float Vec3::dot(const Vec3& other) const
    {
        return glm::dot(ToGlm(*this), ToGlm(other));
    }

    Vec3 Vec3::cross(const Vec3& other) const
    {
        Vec3 result;

        ToGlm(result) = glm::cross(ToGlm(*this), ToGlm(other));

        return result;
    }

    float Vec3::length() const
    {
        return std::sqrt(dot(*this));
    }

    Vec3 Vec3::normalized() const
    {
        Vec3 result;

        ToGlm(result) = glm::normalize(ToGlm(*this));

        return result;
    }

    float Vec3::distance(const Vec3& other) const
    {
        return (*this - other).length();
    }
}
