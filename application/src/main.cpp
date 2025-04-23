#include <exception>
#include <iostream>
#include <functional>
#include <NarcLog.h>

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#ifndef TESTING_BUILD_DLL
#include "Application.h"

narc::Application* g_app = nullptr;

void engineRun()
{
    NARCLOG_PREPARE_HANDLER(NarcEngine);

    NARC_EXECUTE_HANDLED(NarcEngine, g_app = new narc::Application());
    NARC_EXECUTE_HANDLED(NarcEngine, g_app->start());

    NARCLOG_DEBUG("Engine initialized correctly.");

    while (!g_app->shouldClose())
    {
        NARC_EXECUTE_HANDLED(NarcEngine, g_app->appLoopBody());
    }
}

void engineShutdown()
{
    if (g_app == nullptr)
    {
        return;
    }

    NARCLOG_DEBUG("Engine is shutting down");
    g_app->stop();
    delete g_app;
}

int main(int argc, char** argv)
{
    narclog::createLogger();

    const int result = NARCLOG_EXECUTE_FINALLY_FATAL_SAFE(engineRun(), engineShutdown());

    narclog::destroyLogger();
    return result;
}

#endif

#endif