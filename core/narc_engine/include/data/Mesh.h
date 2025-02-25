#pragma once
#include "Vertex.h"
#include "buffers/GraphicsBuffer.h"

namespace narc_engine
{
    class NARC_ENGINE_API Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
        ~Mesh();

        const GraphicsBuffer<Vertex>* getVertexBuffer() const { return &m_vertexBuffer; }
        const GraphicsBuffer<uint16_t>* getIndexBuffer() const { return &m_indexBuffer; }
        uint32_t getIndexCount() const { return m_indexCount; }

    private:
        GraphicsBuffer<Vertex> m_vertexBuffer;
        GraphicsBuffer<uint16_t> m_indexBuffer;

        uint32_t m_indexCount;
    };
}
