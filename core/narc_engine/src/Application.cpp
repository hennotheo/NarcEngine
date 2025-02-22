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
    Engine engine;

    engine.run();
}