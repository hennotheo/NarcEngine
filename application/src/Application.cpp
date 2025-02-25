//
// Created by theoh on 24/02/2025.
//

#include "Application.h"

namespace narc
{
    const std::vector<narc_engine::Vertex> g_vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> g_indices = {
        0, 1, 2, 2, 3, 0
    };

    Application::Application()
    {
        m_engine = new narc_engine::Engine();
    }

    Application::~Application()
    {
        delete m_engine;
    }

    void Application::run()
    {
        const narc_engine::Mesh* mesh = new narc_engine::Mesh(g_vertices, g_indices);
        narc_engine::Engine::binder()->bindMesh(mesh);

        while (!m_engine->shouldClose())
        {
            //PRE-UPDATE ENGINE LOGIC
            m_engine->pollEvents();

            //UPDATE ENGINE LOGIC

            //RENDER ENGINE
            m_engine->render();
        }

        m_engine->waitDeviceIdle();

        delete mesh;
    }
}
