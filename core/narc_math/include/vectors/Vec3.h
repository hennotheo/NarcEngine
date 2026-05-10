//
// Created by theo on 3/12/26.
//


#pragma once

namespace narc_math {
    struct alignas(4) NARC_ENGINE_API Vec3
    {
        union
        {
            struct
            {
                float X, Y, Z;
            };

            float Data[3] = {0, 0, 0};
        };

        Vec3();
        Vec3(const Vec3& vector);
        Vec3(float x, float y, float z);

        ~Vec3();

        bool operator==(const Vec3& other) const;
        bool operator!=(const Vec3& other) const { return !(*this == other); }
        Vec3 operator+(const Vec3& other) const;
        Vec3 operator-(const Vec3& other) const;
        Vec3 operator*(float scalar) const;
        Vec3& operator+=(const Vec3& other);
        Vec3& operator-=(const Vec3& other);
        Vec3& operator*=(float scalar);

        NO_DISCARD float dot(const Vec3& other) const;
        NO_DISCARD Vec3 cross(const Vec3& other) const;
        NO_DISCARD float length() const;
        NO_DISCARD Vec3 normalized() const;
        NO_DISCARD float distance(const Vec3& other) const;

        static Vec3 up();
        static Vec3 down();
        static Vec3 right();
        static Vec3 left();
        static Vec3 forward();
        static Vec3 backward();
    };
}
