#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#ifndef NARC_TEST_BUILD
#include "Application.h"

narc::Application* g_app = nullptr;

void engineRun()
{
    g_app = new narc::Application();
    g_app->start();

    NARCLOG_DEBUG("Engine initialized correctly.");

    while (!g_app->shouldClose())
    {
        g_app->appLoopBody();
    }
}

void engineShutdown()
{
    g_app->stop();
    delete g_app;
}

int main(int argc, char **argv)
{
    NARCLOG_INIT(engineShutdown);
    
    engineRun();
    
    engineShutdown();
    narclog::destroyLogger();
    return 0;
}

#endif

#endif