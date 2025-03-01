//
// Created by theoh on 24/02/2025.
//

#include "Application.h"

#include <NarcLog.h>

namespace narc {
    const std::vector<narc_engine::Vertex> g_vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint16_t> g_indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    const narc_engine::Mesh* g_mesh = nullptr;

    Application::Application()
    {
        m_engine = narc_engine::createEngine();
    }

    Application::~Application()
    {
        delete m_engine;
    }

    bool Application::shouldClose() const
    {
        return m_engine->shouldClose();
    }

    void Application::start()
    {
        g_mesh = new narc_engine::Mesh(g_vertices, g_indices);
        narc_engine::getEngine()->binder()->bindMesh(g_mesh);
    }

    void Application::stop()
    {
        m_engine->waitDeviceIdle();
        delete g_mesh;
    }

    void Application::appLoopBody()
    {
        //PRE-UPDATE ENGINE LOGIC
        m_engine->pollEvents();

        //UPDATE ENGINE LOGIC

        //RENDER ENGINE
        m_engine->render();
    }
}
