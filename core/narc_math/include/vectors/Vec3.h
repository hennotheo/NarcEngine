//
// Created by theo on 3/12/26.
//


#pragma once

namespace narc_math {
    struct alignas(4) NARC_ENGINE_API Vec3
    {
        float Data[3] = {0, 0, 0};

        Vec3();
        Vec3(const Vec3& vector);
        Vec3(float x, float y, float z);

        ~Vec3();


        bool operator==(const Vec3& other) const;
    };
}