#include <exception>
#include <iostream>
#include <functional>
#include <NarcLog.h>

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#ifndef NARC_TEST_BUILD
#include "Application.h"

narc::Application *g_app = nullptr;

void engineRun()
{
    throw std::runtime_error("Test exception");
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
    NARCLOG_DEBUG("Engine is shutting down");
    g_app->stop();
    delete g_app;

    narclog::destroyLogger();
}

int main(int argc, char **argv)
{
    NARCLOG_INIT();

    engineRun();

    engineShutdown();
    return 0;
}

#endif

#endif