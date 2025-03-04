//
// Created by theoh on 3/4/2025.
//

#pragma once

namespace narc_io {
    struct NARC_IO_API Model3D final
    {
        friend class FileReader;

        [[nodiscard]] uint32_t getVerticesCount() const { return m_vertices.size(); };
        [[nodiscard]] const std::vector<std::array<float, 3> >* getVertices() const { return &m_vertices; };
        [[nodiscard]] const std::vector<std::array<float, 2> >* getTexCoords() const { return &m_texCoords; };
        [[nodiscard]] const std::vector<uint32_t>* getIndices() const { return &m_indices; };

    private:
        std::vector<std::array<float, 3> > m_vertices{};
        std::vector<std::array<float, 2> > m_texCoords{};
        std::vector<uint32_t> m_indices;
    };
} // narcio
