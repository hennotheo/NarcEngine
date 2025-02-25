//
// Created by theoh on 25/02/2025.
//
#include "data/Mesh.h"

namespace narc_engine
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
    {
        m_vertexBuffer.create(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        m_indexBuffer.create(indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        m_indexCount = static_cast<uint32_t>(indices.size());
    }

    Mesh::~Mesh()
    {
        m_indexBuffer.release();
        m_vertexBuffer.release();
    }
}
