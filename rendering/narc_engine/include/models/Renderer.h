//
// Created by theoh on 08/03/2025.
//

#pragma once
#include <NarcMath.h>

namespace narc_engine
{
    class Mesh;
    class Material;

    class NARC_ENGINE_API Renderer
    {
    public:
        Renderer(const Mesh* mesh, const Material* material);
        ~Renderer();

        const Material* getMaterial() const { return m_material; }
        const Mesh* getMesh() const { return m_mesh; }

    private:
        const Mesh* m_mesh;
        const Material* m_material;

        glm::mat4 m_modelMatrix;
    };
} // narc_engine