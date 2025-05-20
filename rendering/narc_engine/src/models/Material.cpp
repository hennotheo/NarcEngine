//
// Created by theoh on 08/03/2025.
//

#include "models/Material.h"

#include "Engine.h"

#include "resources/Shader.h"
#include "resources/Texture2DResource.h"

namespace narc_engine
{
    uint32_t Material::s_materialCounter = 0;

    Material::Material(const char* texturePath) :
        Resource(ResourceType::Material), //TODO not create texture in material
        m_mainTexture(Engine::getInstance()->resourceManager()->createResource<Texture2DResource>(texturePath)),
        m_shader(Engine::getInstance()->resourceManager()->createResource<Shader>("shaders/shader_vert.spv", "shaders/shader_frag.spv"))
    {
        s_materialCounter++;
    }

    Material::~Material()
    {
#pragma warning "disable shader and texture unloading" 
    }
} // narc_engine
