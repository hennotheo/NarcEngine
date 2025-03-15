//
// Created by theoh on 08/03/2025.
//

#pragma once

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

        GETTER const glm::mat4& getModelMatrix() const { return m_modelMatrix; }
        TEMP_CODE void setModelMatrix(const glm::mat4& modelMatrix) { m_modelMatrix = modelMatrix; }

    private:
        std::unique_ptr<Mesh> m_mesh;
        const Material* m_material;

        glm::mat4 m_modelMatrix;
    };
} // narc_engine