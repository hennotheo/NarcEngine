//
// Created by theoh on 25/02/2025.
//
#include "models/Mesh.h"

#include "buffers/GraphicsBuffer.h"

namespace narc_engine {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        m_vertexBuffer = std::make_unique<VertexBuffer>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        m_indexBuffer = std::make_unique<IndexBuffer>(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        m_vertexBuffer->create(vertices);
        m_indexBuffer->create(indices);

        m_indexCount = static_cast<uint32_t>(indices.size());
    }

    Mesh::~Mesh() = default;
}