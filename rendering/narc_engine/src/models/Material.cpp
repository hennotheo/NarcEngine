//
// Created by theoh on 08/03/2025.
//

#include "models/Material.h"

namespace narc_engine
{
    Material::Material(const char* texturePath)
        : m_mainTexture(narc_io::FileReader::readImage(texturePath))
    {
    }

    Material::~Material()
    {
    }
} // narc_engine
