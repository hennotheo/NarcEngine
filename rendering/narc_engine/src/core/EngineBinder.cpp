#include "core/EngineBinder.h"

#include "Engine.h"

namespace narc_engine {
    void EngineBinder::bindMesh(const Mesh* mesh) const
    {
        static_cast<Engine*>(m_engine)->m_renderer->bindMesh(mesh);
    }

    void EngineBinder::unbindMesh(const Mesh* mesh) const
    {
        static_cast<Engine*>(m_engine)->m_renderer->unbindMesh(mesh);
    }

    void EngineBinder::bindMaterial(const Material* material) const
    {
        static_cast<Engine*>(m_engine)->m_renderer->bindMaterial(material);
    }
}
