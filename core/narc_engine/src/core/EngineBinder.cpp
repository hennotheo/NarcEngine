#include "core/EngineBinder.h"

#include "Engine.h"

namespace narc_engine {
    void EngineBinder::bindMesh(const Mesh* mesh)
    {
        static_cast<Engine*>(m_engine)->m_renderer.bindMesh(mesh);
    }

    void EngineBinder::unbindMesh(const Mesh* mesh)
    {
        static_cast<Engine*>(m_engine)->m_renderer.unbindMesh(mesh);
    }
} // narc_engine