//
// Created by theoh on 25/02/2025.
//
#include "resources/Mesh.h"

#include "models/Vertex.h"
#include "buffers/GraphicsBuffer.h"

namespace narc_engine {
    Mesh::Mesh(const narc_io::Model3D& model) : Resource(ResourceType::Mesh), m_model(model)
    {

    }

    Mesh::~Mesh() = default;

    void Mesh::onLoad()
    {
        m_vertices.reserve(m_model.getVerticesCount());
        m_indices.reserve(m_model.getIndicesCount());

        const narc_io::VertexList modelVertices = *m_model.getVertices();
        const narc_io::TexCoordList modelTexCoords = *m_model.getTexCoords();
        const narc_io::IndexList modelIndices = *m_model.getIndices();
        for (uint32_t i = 0; i < m_model.getVerticesCount(); ++i)
        {
            m_vertices.push_back({
                {modelVertices[i][0], modelVertices[i][1], modelVertices[i][2]},
                {1.0f, 1.0f, 1.0f},
                {modelTexCoords[i][0], modelTexCoords[i][1]}
                });
        }
        for (uint32_t modelIndice : modelIndices)
        {
            m_indices.push_back(modelIndice);
        }

        m_vertexBuffer = std::make_unique<VertexBuffer>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        m_indexBuffer = std::make_unique<IndexBuffer>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        m_vertexBuffer->create(m_vertices);
        m_indexBuffer->create(m_indices);

        m_indexCount = static_cast<uint32_t>(m_indices.size());
    }

    void Mesh::onUnload()
    {
        m_vertexBuffer.reset();
        m_indexBuffer.reset();
        m_vertices.clear();
        m_indices.clear();
        m_indexCount = 0;
    }
}