#pragma once

#include "NarcMath.h"

namespace narc_math
{
    class NARC_ENGINE_API Transform
    {
    public:
        Transform()
        {
            m_modelMatrix = glm::mat4(1.0f);
            m_modelMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }

        ~Transform()
        {
        }

        NARC_GETTER(const glm::vec3&, getWorldPosition, glm::vec3(m_modelMatrix[3]));
        NARC_GETTER(const glm::vec3&, getWorldRotation, glm::vec3(m_modelMatrix[0]));
        NARC_GETTER(const glm::vec3&, getWorldScale, glm::vec3(m_modelMatrix[0][0], m_modelMatrix[1][1], m_modelMatrix[2][2]));
        NARC_GETTER(const glm::mat4&, getModelMatrix, m_modelMatrix);

        inline void setWorldPosition(const glm::vec3& position)
        {
            m_modelMatrix[3] = glm::vec4(position, 1.0f);
        }

        inline void translate(const glm::vec3& translation)
        {
            m_modelMatrix = glm::translate(m_modelMatrix, translation);
        }

        inline void setWorldScale(const glm::vec3& scale)
        {
            m_modelMatrix[0][0] = scale.x;
            m_modelMatrix[1][1] = scale.y;
            m_modelMatrix[2][2] = scale.z;
        }

        inline void rotateAxis(const float radians, const glm::vec3& axis)
        {
            m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(radians), axis);
        }

        inline void rotateX(const float radians) { rotateAxis(radians, glm::vec3(1.0f, 0.0f, 0.0f)); }
        inline void rotateY(const float radians) { rotateAxis(radians, glm::vec3(0.0f, 1.0f, 0.0f)); }
        inline void rotateZ(const float radians) { rotateAxis(radians, glm::vec3(0.0f, 0.0f, 1.0f)); }

    private:
        glm::mat4 m_modelMatrix;
    };
}