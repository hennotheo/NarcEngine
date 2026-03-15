//
// Created by theoh on 3/4/2025.
//

#pragma once


namespace narc_io
{
    typedef std::vector<narc_math::Vec3> VertexList;
    typedef std::vector<narc_math::Vec3> ColorList;
    typedef std::vector<narc_math::Vec2> TexCoordList;
    typedef std::vector<uint32_t> IndexList;

    struct NARC_IO_API Model3D final
    {
        friend class FileReaderService;

        NARC_GETTER(uint32_t, getVerticesCount, m_vertices.size());
        NARC_GETTER(const VertexList&, getVertices, m_vertices);

        NARC_GETTER(uint32_t, getIndicesCount, m_indices.size());
        NARC_GETTER(const IndexList&, getIndices, m_indices);

        NARC_GETTER(const TexCoordList&, getTexCoords, m_texCoords);
        NARC_GETTER(const ColorList&, getColors, m_colors);

    private:
        std::vector<narc_math::Vec3> m_vertices{};
        std::vector<narc_math::Vec2> m_texCoords{};
        std::vector<narc_math::Vec3> m_colors{};
        std::vector<uint32_t> m_indices;
    };
} // narcio
