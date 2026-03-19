//
// Created by theo on 3/18/26.
//

#include "Quaternion.h"

#include "GlmCore.h"

namespace narc_math {
    Quaternion::Quaternion() = default;

    Quaternion::Quaternion(const Quaternion& other)
    {
        memcpy(Data, other.Data, sizeof(Quaternion));
    }

    Quaternion::Quaternion(const float x, const float y, const float z, const float w)
    {
        X = x;
        Y = y;
        Z = z;
        W = w;
    };

    bool Quaternion::operator==(const Quaternion& other) const
    {
        return ToGlm(*this) == ToGlm(other);
    }

    Quaternion Quaternion::operator*(const Quaternion& other) const
    {
        Quaternion result;

        ToGlm(result) = ToGlm(*this) * ToGlm(other);

        return result;
    }

    Vec3 Quaternion::toEulerAngles() const
    {
        Vec3 result;

        ToGlm(result) = glm::eulerAngles(ToGlm(*this));

        return result;
    }

    float Quaternion::length() const
    {
        return glm::length(ToGlm(*this));;
    }

    Quaternion Quaternion::normalized() const
    {
        Quaternion result;

        ToGlm(result) = glm::normalize(ToGlm(*this));

        return result;
    }

    float Quaternion::dot(const Quaternion& v) const
    {
        return dot(*this, v);
    }

    Vec3 Quaternion::rotate(const Vec3& v) const
    {
        Vec3 result;

        ToGlm(result) = glm::rotate(ToGlm(*this), ToGlm(v));

        return result;
    }

    Quaternion Quaternion::identity()
    {
        Quaternion result;

        ToGlm(result) = glm::identity<glm::quat>();

        return result;
    }

    Quaternion Quaternion::lookRotation(const Vec3& relativePos, const Vec3& up)
    {
        Quaternion result;

        ToGlm(result) = glm::quatLookAt(ToGlm(relativePos), ToGlm(up));

        return result;
    }

    Quaternion Quaternion::fromAxisAngle(const Vec3& axis, float angle)
    {
        Quaternion result;

        ToGlm(result) = glm::angleAxis(angle, ToGlm(axis));

        return result;
    }

    float Quaternion::dot(const Quaternion& a, const Quaternion& b)
    {
        return glm::dot(ToGlm(a), ToGlm(b));;
    }

    Quaternion Quaternion::lerp(const Quaternion& a, const Quaternion& b, float t)
    {
        Quaternion result;

        ToGlm(result) = glm::lerp(ToGlm(a), ToGlm(b), t);

        return result;
    }

    Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, float t)
    {
        Quaternion result;

        ToGlm(result) = glm::slerp(ToGlm(a), ToGlm(b), t);

        return result;
    }
}
