#include "include/Application.h"
#include "include/Engine.h"
#include "include/window/Window.h"

NarcEngine::Application::Application()
{
}

NarcEngine::Application::~Application()
{
}

void NarcEngine::Application::Run()
{
    Engine engine;

    engine.Run();

    std::cin.get();
}