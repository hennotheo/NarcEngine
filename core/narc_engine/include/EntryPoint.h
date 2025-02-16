#pragma once

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#include "pch.h"

extern narc_engine::Application* narc_engine::createApplication();

int main(int argc, char** argv)
{
	auto engine = narc_engine::createApplication();

	try
	{
		engine->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cin.get();
		
		delete engine;
		return EXIT_FAILURE;
	}

	delete engine;
	return EXIT_SUCCESS;
}

#endif