#include "include/Application.h"

#include <iostream>

#include "include/Engine.h"

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