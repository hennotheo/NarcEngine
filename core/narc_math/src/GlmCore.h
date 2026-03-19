//
// Created by theo on 3/12/26.
//


#pragma once

#include "NarcMath.h"

#define GLM_FORCE_COLUMN_MAJOR
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/quaternion.hpp>

namespace narc_math {
    static glm::mat4& ToGlm(Matrix4& matrix)
    {
        return *reinterpret_cast<glm::mat4*>(matrix.Data);
    }

    static const glm::mat4& ToGlm(const Matrix4& matrix)
    {
        return *reinterpret_cast<const glm::mat4*>(matrix.Data);
    }

    static const glm::vec3& ToGlm(const Vec3& vector)
    {
        return reinterpret_cast<const glm::vec3&>(vector);
    }

    static glm::vec3& ToGlm(Vec3& vector)
    {
        return reinterpret_cast<glm::vec3&>(vector);
    }

    static glm::vec4& ToGlm(Vec4& vector)
    {
        return reinterpret_cast<glm::vec4&>(vector);
    }

    static const glm::vec4& ToGlm(const Vec4& vector)
    {
        return reinterpret_cast<const glm::vec4&>(vector);
    }

    static glm::quat& ToGlm(Quaternion& quaternion)
    {
        return reinterpret_cast<glm::quat&>(quaternion);
    }

    static const glm::quat& ToGlm(const Quaternion& quaternion)
    {
        return reinterpret_cast<const glm::quat&>(quaternion);
    }
}
