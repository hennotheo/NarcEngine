//
// Created by theo on 3/12/26.
//

#include "vectors/Vec3.h"

namespace narc_math {

    Vec3::Vec3() = default;

    Vec3::Vec3(const Vec3& vector)
    {
        Data[0] = vector.Data[0];
        Data[1] = vector.Data[1];
        Data[2] = vector.Data[2];
    }

    Vec3::Vec3(float x, float y, float z)
    {
        Data[0] = x;
        Data[1] = y;
        Data[2] = z;
    }

    Vec3::~Vec3()
    {
    }

    bool Vec3::operator==(const Vec3& other) const
    {
        return Data[0] == other.Data[0] && Data[1] == other.Data[1] && Data[2] == other.Data[2];
    }
}
