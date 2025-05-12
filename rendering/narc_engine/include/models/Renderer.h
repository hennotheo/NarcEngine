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
        explicit Renderer(const narc_io::Model3D* model, ResourceId material, const narc_math::Transform* transform);
        ~Renderer();

        GETTER ResourceId getMaterial() const { return m_material; }
        GETTER const Mesh* getMesh() const { return m_mesh; }

        TEMP_CODE GETTER const glm::mat4 getModelMatrix() const { return m_transform->getModelMatrix(); }

    private:
        const Mesh* m_mesh;
        ResourceId m_material;

        const narc_math::Transform* m_transform = nullptr;
    };
} // narc_engine