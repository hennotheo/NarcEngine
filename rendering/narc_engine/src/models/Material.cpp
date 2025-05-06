//
// Created by theoh on 08/03/2025.
//

#include "models/Material.h"

#include "Engine.h"
#include "models/Shader.h"

namespace narc_engine
{
    uint32_t Material::s_materialCounter = 0;

    Material::Material(const char* texturePath) : //TODO not create texture in material
        m_mainTexture(Engine::getInstance()->resourceManager()->createTexture2D(texturePath))
    {
        m_materialID = s_materialCounter;
        s_materialCounter++;

        m_vertShaderModule = new Shader("shaders/shader_vert.spv");
        m_fragShaderModule = new Shader("shaders/shader_frag.spv");
    }

    Material::~Material()
    {
        delete m_vertShaderModule;
        delete m_fragShaderModule;
    }
} // narc_engine
