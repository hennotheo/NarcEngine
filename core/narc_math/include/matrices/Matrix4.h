//
// Created by theo on 3/12/26.
//


#pragma once

namespace narc_math {
    struct Vec4;
    struct Vec3;

    struct alignas(16) NARC_ENGINE_API Matrix4
    {
        float Data[16];

        Matrix4();
        ~Matrix4();

        Matrix4 operator*(const Matrix4& other) const;
        Vec4 operator*(const Vec4& other) const;
        float& operator()(size_t row, size_t col);
        const float& operator()(size_t row, size_t col) const;

        static Matrix4 identity();
        static Matrix4 perspective(float fov, float aspect, float nearPlane, float farPlane);

        Matrix4 rotate(const Vec3& axis, float angle);
        Matrix4 translation(const Vec3& delta);
        Matrix4 scale(const Vec3& scale);
        Matrix4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    };
}
