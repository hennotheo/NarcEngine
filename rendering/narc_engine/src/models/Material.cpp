//
// Created by theoh on 08/03/2025.
//

#include "models/Material.h"

#include "Engine.h"
#include "models/Shader.h"

#include "resources/Texture2DResource.h"

namespace narc_engine
{
    uint32_t Material::s_materialCounter = 0;

    Material::Material(const char* texturePath) :
        Resource(ResourceType::Material), //TODO not create texture in material
        m_mainTexture(Engine::getInstance()->resourceManager()->createResource<Texture2DResource>(texturePath))
    {
        s_materialCounter++;

        m_shader = new Shader(std::string("shaders/shader_vert.spv"), std::string("shaders/shader_frag.spv"));
    }

    Material::~Material()
    {
        delete m_shader;
    }
} // narc_engine
