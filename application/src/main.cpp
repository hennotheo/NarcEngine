#include <exception>
#include <iostream>
#include <functional>
#include <NarcLog.h>

#ifdef NARC_ENGINE_PLATFORM_WINDOWS
#include "Application.h"

int main(int argc, char** argv)
{
    narclog::createLogger();
    narc::Application* app = new narc::Application();
    app->start();

    while (!app->shouldClose())
    {
        narclog::executeWithExceptionHandling(std::bind(&narc::Application::appLoopBody, app));
    }

    app->stop();
    delete app;
    narclog::destroyLogger();
    return EXIT_SUCCESS;
}

#endif
