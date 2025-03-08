//
// Created by theoh on 08/03/2025.
//

#include "models/Material.h"

namespace narc_engine
{
    uint32_t Material::s_materialCounter = 0;

    Material::Material(const char* texturePath)
        : m_mainTexture(narc_io::FileReader::readImage(texturePath))
    {
        m_materialID = s_materialCounter;
        s_materialCounter++;
    }

    Material::~Material()
    {
    }
} // narc_engine
