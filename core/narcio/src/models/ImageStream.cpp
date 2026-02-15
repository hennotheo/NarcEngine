//
// Created by theo on 12/22/25.
//

#include "models/ImageStream.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "NarcLog.h"

namespace narc_io {
    ImageStream::ImageStream(const std::string_view path) :
        m_path(path)
    {
        m_pixels = stbi_load(m_path.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);

        if (!m_pixels)
        {
            NARC_ERROR_RUNTIME("Failed to load texture image!");
        }
    }

    ImageStream::~ImageStream()
    {
        stbi_image_free(m_pixels);
    }
}
