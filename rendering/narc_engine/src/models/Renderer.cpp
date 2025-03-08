//
// Created by theoh on 08/03/2025.
//

#include "models/Renderer.h"

#include "Engine.h"
#include "NarcLog.h"

namespace narc_engine
{
    Renderer::Renderer(const Mesh* mesh, const Material* material)
    {
        m_mesh = mesh;
        m_material = material;

        m_modelMatrix = glm::mat4();

        Engine::getInstance()->binder()->attachRenderer(this);
    }

    Renderer::~Renderer()
    {
    }
} // narc_engine
