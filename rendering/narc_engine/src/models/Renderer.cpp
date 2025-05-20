//
// Created by theoh on 08/03/2025.
//

#include "models/Renderer.h"

#include "Engine.h"

namespace narc_engine
{
    Renderer::Renderer(const narc_io::Model3D& model, ResourceId material, const narc_math::Transform* transform)
    {
        m_mesh = Engine::getInstance()->resourceManager()->createResource<Mesh>(model);
        m_material = material;
        m_transform = transform;

        Engine::getInstance()->binder()->attachRenderer(this);
    }

    Renderer::~Renderer()
    {
        
    }
} // narc_engine
