#include "Application.h"

#include "Engine.h"

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
}