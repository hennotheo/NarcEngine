#include "FileReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <NarcLog.h>

#include "models/Image.h"
#include "models/Model3D.h"

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

        std::vector<std::array<float, 3> > vertices{};
        std::vector<std::array<float, 2> > texCoords{};
        std::vector<uint32_t> indices;

        for (const auto& shape: shapes)
        {
            for (const auto& index: shape.mesh.indices)
            {
                vertices.push_back({
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                });
                texCoords.push_back({
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                });

                indices.push_back(indices.size());
            }
        }

        Model3D model;
        model.m_vertices = vertices;
        model.m_texCoords = texCoords;
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
