#pragma once

namespace narc_engine {
    struct Vertex;

    template<typename T>
    class GraphicsBuffer;

    typedef GraphicsBuffer<Vertex> VertexBuffer;
    typedef GraphicsBuffer<uint32_t> IndexBuffer;

    class NARC_ENGINE_API Mesh
    {
    public:
        explicit Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        Mesh(const Mesh&) = delete;
        ~Mesh();

        const VertexBuffer* getVertexBuffer() const { return m_vertexBuffer.get(); }
        const IndexBuffer* getIndexBuffer() const { return m_indexBuffer.get(); }
        uint32_t getIndexCount() const { return m_indexCount; }

    private:
        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;

        uint32_t m_indexCount;
    };
}
