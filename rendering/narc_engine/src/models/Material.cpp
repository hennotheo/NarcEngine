//
// Created by theoh on 08/03/2025.
//

#include "models/Material.h"

#include "Engine.h"

namespace narc_engine
{
    uint32_t Material::s_materialCounter = 0;

    Material::Material(const char* texturePath): //TODO not create texture in material
        m_mainTexture(Engine::getInstance()->resourceManager()->createTexture2D(texturePath))
    {
        m_materialID = s_materialCounter;
        s_materialCounter++;
    }

    Material::~Material() = default;
} // narc_engine
