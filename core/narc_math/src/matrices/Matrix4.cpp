//
// Created by theo on 3/12/26.
//

#include "matrices/Matrix4.h"

#include "vectors/Vec3.h"

#include "../GlmCore.h"
#include "vectors/Vec4.h"

namespace narc_math {
    static_assert(sizeof(glm::mat4) == sizeof(Matrix4));
    static_assert(sizeof(glm::mat4) == 64);
    static_assert(sizeof(Matrix4) == 64);

    Matrix4::Matrix4() = default;

    Matrix4::~Matrix4() = default;

    Matrix4 Matrix4::operator*(const Matrix4& other) const
    {
        Matrix4 result;

        ToGlm(result) = ToGlm(*this) * ToGlm(other);

        return result;
    }

    Vec4 Matrix4::operator*(const Vec4& other) const
    {
        Vec4 result;

        ToGlm(result) = ToGlm(*this) * ToGlm(other);

        return result;
    }

    float& Matrix4::operator()(const size_t row, const size_t col)
    {
        return Data[row * 4 + col];
    }

    const float& Matrix4::operator()(const size_t row, const size_t col) const
    {
        return Data[row * 4 + col];
    }

    Matrix4 Matrix4::identity()
    {
        Matrix4 result;
        ToGlm(result) = glm::mat4(1.0f);

        return result;
    }

    Matrix4 Matrix4::perspective(const float fov, const float aspect, const float nearPlane, const float farPlane)
    {
        Matrix4 result;

        ToGlm(result) = glm::perspective(fov, aspect, nearPlane, farPlane);

        return result;
    }

    Matrix4 Matrix4::model(const Vec3& delta, const Quaternion& rotation, const Vec3& scale)
    {
        const Matrix4 translation = identity().translation(delta);
        const Matrix4 rot = rotation.normalized().toMatrix();
        const Matrix4 scaleMat = identity().scale(scale);

        return translation * rot * scaleMat;
    }

    Matrix4 Matrix4::rotate(const Vec3& axis, const float angle)
    {
        Matrix4 result;

        ToGlm(result) = glm::rotate(ToGlm(*this), angle, ToGlm(axis));

        return result;
    }

    Matrix4 Matrix4::translation(const Vec3& delta)
    {
        Matrix4 result;

        ToGlm(result) = glm::translate(ToGlm(*this), ToGlm(delta));

        return result;
    }

    Matrix4 Matrix4::scale(const Vec3& scale)
    {
        Matrix4 result;

        ToGlm(result) = glm::scale(ToGlm(*this), ToGlm(scale));

        return result;
    }

    Matrix4 Matrix4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
        const Vec3 f = (center - eye).normalized();
        const Vec3 s = f.cross(up).normalized();
        const Vec3 u = s.cross(f);

        Matrix4 result = identity();


        result(0,0) = s.X;
        result(1,0) = s.Y;
        result(2,0) = s.Z;

        result(0,1) = u.X;
        result(1,1) = u.Y;
        result(2,1) = u.Z;

        result(0,2) = -f.X;
        result(1,2) = -f.Y;
        result(2,2) = -f.Z;

        result(3,0) = -s.dot(eye);
        result(3,1) = -u.dot(eye);
        result(3,2) =  f.dot(eye);

        return result;
    }
}
