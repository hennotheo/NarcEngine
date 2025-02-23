#include "Application.h"

#include "Engine.h"

narc_engine::Application::Application()
{
}

narc_engine::Application::~Application()
{
}

void narc_engine::Application::run()
{
    Engine* engine = new Engine();

    while (!engine->shouldClose())
    {
        //PRE-UPDATE ENGINE LOGIC
        engine->pollEvents();

        //UPDATE ENGINE LOGIC

        //RENDER ENGINE
        engine->render();
    }

    delete engine;
}
