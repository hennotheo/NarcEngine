//
// Created by theo on 3/12/26.
//

#include "matrices/Matrix4.h"

#include "vectors/Vec3.h"

#include "../GlmCore.h"

namespace narc_math {
    static_assert(sizeof(glm::mat4) == sizeof(Matrix4));
    static_assert(sizeof(glm::mat4) == 64);
    static_assert(sizeof(Matrix4) == 64);

    static glm::mat4& ToGlm(Matrix4& matrix)
    {
        return *reinterpret_cast<glm::mat4*>(matrix.Data);
    }

    static const glm::mat4& ToGlm(const Matrix4& matrix)
    {
        return *reinterpret_cast<const glm::mat4*>(matrix.Data);
    }

    static const glm::vec3& ToGlmV(const Vec3& vector)
    {
        return reinterpret_cast<const glm::vec3&>(vector);
    }

    Matrix4::Matrix4() = default;

    Matrix4::~Matrix4() = default;

    Matrix4 Matrix4::operator*(const Matrix4& other) const
    {
        Matrix4 result;

        ToGlm(result) = ToGlm(*this) * ToGlm(other);

        return result;
    }

    Matrix4 Matrix4::Identity()
    {
        Matrix4 result;
        ToGlm(result) = glm::mat4(1.0f);
        return result;
    }

    Matrix4 Matrix4::Perspective(const float fov, const float aspect, const float nearPlane, const float farPlane)
    {
        Matrix4 result;

        ToGlm(result) = glm::perspective(fov, aspect, nearPlane, farPlane);

        return result;
    }

    Matrix4 Matrix4::rotate(const Vec3& axis, float angle)
    {
        Matrix4 result;

        ToGlm(result) = glm::rotate(ToGlm(*this), angle, ToGlmV(axis));

        return result;
    }

    Matrix4 Matrix4::translation(const Vec3& delta)
    {
        Matrix4 result;

        ToGlm(result) = glm::translate(ToGlm(*this), ToGlmV(delta));

        return result;
    }

    Matrix4 Matrix4::scale(const Vec3& scale)
    {
        Matrix4 result;

        ToGlm(result) = glm::scale(ToGlm(*this), ToGlmV(scale));

        return result;
    }
}
