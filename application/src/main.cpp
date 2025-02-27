#include <exception>
#include <iostream>
#include <functional>
#include <NarcLog.h>

#ifdef NARC_ENGINE_PLATFORM_WINDOWS
#include "Application.h"

narc::Application* g_app = nullptr;

void engineRun()
{
    g_app = new narc::Application();
    g_app->start();

    NARCLOG_PREPARE_HANDLER(NarcEngine);

    while (!g_app->shouldClose())
    {
        NARC_EXECUTE_HANDLED(NarcEngine, g_app->appLoopBody());
    }
}

void engineShutdown()
{
    NARCLOG_DEBUG("SHUTTING DOWN ENGINE");
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
