#include "FileReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <NarcLog.h>
#include "data/Image.h"

namespace narc_io
{
    std::vector<char> FileReader::readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            NARCLOG_FATAL("Failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
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
