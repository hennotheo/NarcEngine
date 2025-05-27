//
// Created by theoh on 08/03/2025.
//

#pragma once

namespace narc_engine
{
    class Mesh;

    class NARC_ENGINE_API Renderer
    {
    public:
        explicit Renderer(const narc_io::Model3D& model, ResourceId material, const narc_math::Transform* transform);
        ~Renderer();

        NARC_GETTER(ResourceId, getMaterial, m_material);
        NARC_GETTER(ResourceId, getMesh, m_mesh);

        TEMP_CODE NARC_GETTER(const glm::mat4&, getModelMatrix, m_transform->getModelMatrix());

    private:
        ResourceId m_mesh;
        ResourceId m_material;

        const narc_math::Transform* m_transform = nullptr;
    };
} // narc_engine
