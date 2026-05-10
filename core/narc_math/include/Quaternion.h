//
// Created by theo on 3/18/26.
//


#pragma once

namespace narc_math {
    struct Matrix4;
    struct Vec3;

    struct NARC_ENGINE_API Quaternion
    {
        union
        {
            struct
            {
                float X, Y, Z, W;
            };

            float Data[4]{0, 0, 0, 0};
        };

        Quaternion();
        Quaternion(const Quaternion& other);
        Quaternion(float x, float y, float z, float w);

        bool operator==(const Quaternion& other) const;
        bool operator!=(const Quaternion& other) const { return !operator==(other); }

        Quaternion operator*(const Quaternion& other) const;
        Quaternion operator*=(const Quaternion& other) const { return *this * other; }

        NO_DISCARD Vec3 toEulerAngles() const;
        NO_DISCARD float length() const;
        NO_DISCARD Quaternion normalized() const;
        NO_DISCARD Matrix4 toMatrix() const;
        NO_DISCARD float dot(const Quaternion& v) const;
        NO_DISCARD Vec3 rotate(const Vec3& v) const;

        static Quaternion identity();
        static Quaternion lookRotation(const Vec3& relativePos, const Vec3& up);
        static Quaternion fromAxisAngle(const Vec3& axis, float angle);
        static float dot(const Quaternion& a, const Quaternion& b);

        static Quaternion lerp(const Quaternion& a, const Quaternion& b, float t);
        static Quaternion slerp(const Quaternion& a, const Quaternion& b, float t);
    };
}
