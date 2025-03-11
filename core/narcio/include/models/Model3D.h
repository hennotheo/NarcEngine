//
// Created by theoh on 3/4/2025.
//

#pragma once

#include <NarcMath.h>

namespace narc_io {
    typedef std::vector<glm::vec3> VertexList;
    typedef std::vector<glm::vec3> ColorList;
    typedef std::vector<glm::vec2> TexCoordList;
    typedef std::vector<uint32_t> IndexList;

    struct NARC_IO_API Model3D final
    {
        friend class FileReader;

        GETTER uint32_t getVerticesCount() const { return m_vertices.size(); };
        GETTER const VertexList* getVertices() const { return &m_vertices; };
        GETTER const TexCoordList* getTexCoords() const { return &m_texCoords; };
        GETTER const ColorList* getColors() const { return &m_colors; };
        GETTER const IndexList* getIndices() const { return &m_indices; };

    private:
        std::vector<glm::vec3> m_vertices{};
        std::vector<glm::vec2> m_texCoords{};
        std::vector<glm::vec3> m_colors{};
        std::vector<uint32_t> m_indices;
    };
} // narcio
