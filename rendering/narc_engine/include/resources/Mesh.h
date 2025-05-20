#pragma once

#include "resources/Resource.h"

namespace narc_engine {
    struct Vertex;

    template<typename T>
    class GraphicsBuffer;

    typedef GraphicsBuffer<Vertex> VertexBuffer;
    typedef GraphicsBuffer<uint32_t> IndexBuffer;

    class NARC_ENGINE_API Mesh : public Resource
    {
    public:
        Mesh(const Mesh&) = delete;
        ~Mesh() override;

        const VertexBuffer* getVertexBuffer() const { return m_vertexBuffer.get(); }
        const IndexBuffer* getIndexBuffer() const { return m_indexBuffer.get(); }
        uint32_t getIndexCount() const { return m_indexCount; }

    protected:
        NARC_RESOURCE_IMPLEMENTATION(Mesh, const narc_io::Model3D& model);

    private:
        const narc_io::Model3D& m_model;

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;

        uint32_t m_indexCount;
    };
}
