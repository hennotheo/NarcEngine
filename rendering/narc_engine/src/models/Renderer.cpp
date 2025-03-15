//
// Created by theoh on 08/03/2025.
//

#include "models/Renderer.h"

#include "Engine.h"

namespace narc_engine
{
    Renderer::Renderer(const narc_io::Model3D* model, const Material* material)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        const narc_io::VertexList modelVertices = *model->getVertices();
        const narc_io::TexCoordList modelTexCoords = *model->getTexCoords();
        const narc_io::IndexList modelIndices = *model->getIndices();
        for (uint32_t i = 0; i < model->getVerticesCount(); ++i)
        {
            vertices.push_back({
                {modelVertices[i][0], modelVertices[i][1], modelVertices[i][2]},
                {1.0f, 1.0f, 1.0f},
                {modelTexCoords[i][0], modelTexCoords[i][1]}
            });
        }
        for (uint32_t modelIndice : modelIndices)
        {
            indices.push_back(modelIndice);
        }

        m_mesh = std::make_unique<Mesh>(vertices, indices);
        m_material = material;

        m_modelMatrix = glm::mat4(1.0f);

        Engine::getInstance()->binder()->attachRenderer(this);
    }

    Renderer::~Renderer()
    {
    }
} // narc_engine
