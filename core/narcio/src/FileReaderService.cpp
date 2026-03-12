#include "FileReaderService.h"

#include <NarcLog.h>
#include <NarcMath.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "models/ImageStream.h"
#include "models/Model3D.h"

struct vertex
{
    narc_math::Vec3 pos;
    narc_math::Vec3 color;
    glm::vec2 tex;

    bool operator==(const vertex& other) const { return pos == other.pos && tex == other.tex && color == other.color; }
};

namespace std {
    template<>
    struct hash<narc_math::Vec3>
    {
        size_t operator()(narc_math::Vec3 const& vector) const
        {
            return ((hash<float>()(vector.Data[0]) ^ (hash<float>()(vector.Data[1]) << 1)) >> 1) ^ (hash<float>()(vector.Data[2]) << 1);
        }
    };

    template<>
    struct hash<vertex>
    {
        size_t operator()(vertex const& vertex) const
        {
            return ((hash<narc_math::Vec3>()(vertex.pos) ^ (hash<narc_math::Vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.tex) << 1);
        }
    };
} // namespace std

namespace narc_io {
    std::vector<char> FileReaderService::readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            NARC_ERROR_RUNTIME("Failed to open file!");
        }

        const auto fileSize = file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    Model3D FileReaderService::load3DModel(const std::string& filename)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
        {
            NARC_ERROR_RUNTIME(warn + err);
        }


        std::unordered_map<vertex, uint32_t> uniqueVertices{};
        VertexList vertices;
        TexCoordList texCoords;
        ColorList colors;
        IndexList indices;
        for (const auto& shape: shapes)
        {
            for (const auto& index: shape.mesh.indices)
            {
                vertex vertex{};
                vertex.pos = {attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1],
                              attrib.vertices[3 * index.vertex_index + 2]};

                vertex.tex = {attrib.texcoords[2 * index.texcoord_index + 0], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

                if (uniqueVertices.find(vertex) == uniqueVertices.end())
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex.pos);
                    texCoords.push_back(vertex.tex);
                    colors.push_back({1.0f, 1.0f, 1.0f});
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }

        Model3D model;
        model.m_vertices = vertices;
        model.m_texCoords = texCoords;
        model.m_colors = colors;
        model.m_indices = indices;

        return model;
    }

    std::unique_ptr<IImageStream> FileReaderService::readImage(const std::string& filename)
    {
        return std::make_unique<ImageStream>(filename);
    }
} // namespace narc_io
