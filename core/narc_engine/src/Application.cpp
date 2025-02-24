#include "Application.h"

#include "Engine.h"

namespace narc_engine
{
    const std::vector<Vertex> g_vertices = {
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
    }

    Application::~Application()
    {
    }

    void Application::run()
    {
        Engine* engine = new Engine();
        const Mesh* mesh = new Mesh(g_vertices, g_indices);
        engine->getRenderer()->bindMesh(mesh);

        while (!engine->shouldClose())
        {
            //PRE-UPDATE ENGINE LOGIC
            engine->pollEvents();

            //UPDATE ENGINE LOGIC

            //RENDER ENGINE
            engine->render();
        }

        engine->getDevice()->waitDeviceIdle();

        delete mesh;
        delete engine;
    }
}
