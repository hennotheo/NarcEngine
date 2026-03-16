//
// Created by theo on 3/16/26.
//

#include "vectors/Vec4.h"

#include "../GlmCore.h"
#include "matrices/Matrix4.h"

namespace narc_math {

    Vec4::Vec4() = default;

    Vec4::Vec4(const Vec4& vector)
    {
        Data[0] = vector.Data[0];
        Data[1] = vector.Data[1];
        Data[2] = vector.Data[2];
        Data[3] = vector.Data[3];
    }

    Vec4::Vec4(const float x, const float y, const float z, const float w)
    {
        Data[0] = x;
        Data[1] = y;
        Data[2] = z;
        Data[3] = w;
    }

    Vec4::~Vec4() = default;

    Vec4 Vec4::operator*(const float scalar) const
    {
        Vec4 result;

        ToGlm(result) = ToGlm(*this) * scalar;

        return result;
    }

    Vec4& Vec4::operator*=(const float scalar)
    {
        Vec4 result;

        ToGlm(*this) = ToGlm(*this) * scalar;

        return *this;
    }
}
