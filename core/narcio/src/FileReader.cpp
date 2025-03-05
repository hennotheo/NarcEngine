#include "FileReader.h"

#include <NarcLog.h>
#include <NarcMath.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "models/Image.h"
#include "models/Model3D.h"

struct vertex
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 tex;

    bool operator==(const vertex& other) const
    {
        return pos == other.pos && tex == other.tex && color == other.color;
    }
};

namespace std {
    template<>
    struct hash<vertex>
    {
        size_t operator()(vertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.pos) ^
                     (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.tex) << 1);
        }
    };
}

namespace narc_io {
    std::vector<char> FileReader::readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            NARCLOG_FATAL("Failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    Model3D FileReader::load3DModel(const std::string& filename)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
        {
            NARCLOG_FATAL(warn + err);
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
                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.tex = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                if (!uniqueVertices.contains(vertex))
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

    Image FileReader::readImage(const std::string& filename)
    {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if (!pixels)
        {
            NARCLOG_FATAL("Failed to load texture image!");
        }

        Image image(texWidth, texHeight, texChannels, pixels);

        return image;
    }

    void FileReader::releaseImage(void* imageData)
    {
        stbi_image_free(imageData);
    }
}
