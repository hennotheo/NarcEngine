//
// Created by theoh on 08/03/2025.
//

#pragma once
#include <NarcMath.h>
#include <NarcEngine.h>
#include <NarcIO.h>

namespace narc_engine
{
    class Mesh;
    class Material;

    class NARC_ENGINE_API Renderer
    {
    public:
        Renderer(const narc_io::Model3D* model, const Material* material);
        ~Renderer();

        const Material* getMaterial() const { return m_material; }
        const Mesh* getMesh() const { return m_mesh.get(); }

    private:
        std::unique_ptr<Mesh> m_mesh;
        const Material* m_material;

        glm::mat4 m_modelMatrix;
    };
} // narc_engine