#pragma once
#include "Vertex.h"
#include "buffers/GraphicsBuffer.h"

namespace narc_engine
{
    class Mesh
    {
    public:
        inline Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
        {
            m_vertexBuffer.create(vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            m_indexBuffer.create(indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

            m_indexCount = static_cast<uint32_t>(indices.size());
        }

        inline ~Mesh()
        {
            m_indexBuffer.release();
            m_vertexBuffer.release();
        }

        const GraphicsBuffer<Vertex>* getVertexBuffer() const { return &m_vertexBuffer; }
        const GraphicsBuffer<uint16_t>* getIndexBuffer() const { return &m_indexBuffer; }
        uint32_t getIndexCount() const { return m_indexCount; }

    private:
        GraphicsBuffer<Vertex> m_vertexBuffer;
        GraphicsBuffer<uint16_t> m_indexBuffer;

        uint32_t m_indexCount;
    };
}
