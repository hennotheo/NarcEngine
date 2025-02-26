#include <exception>
#include <iostream>

#ifdef NARC_ENGINE_PLATFORM_WINDOWS
#include "Application.h"

int main(int argc, char** argv)
{
    narc::Application* app = new narc::Application();

    try
    {
        app->run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cout << "Wait for closing application..." << std::endl;
        std::cin.get();

        delete app;
        return EXIT_FAILURE;
    }

    delete app;
}

#endif
