#include <exception>
#include <iostream>
#include <functional>
#include <NarcLog.h>

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
    if (g_app == nullptr)
    {
        return;
    }

    NARCLOG_DEBUG("Engine is shutting down");
    g_app->stop();
    delete g_app;
    
    narclog::destroyLogger();
}

int main(int argc, char** argv)
{
    narclog::createLogger();

    try
    {
        engineRun();
    }
    catch (const narclog::FatalException& e)
    {
        NARCLOG_FATAL("Fatal error: " + std::string(e.what()));
        engineShutdown();
        return -1;
    }
    catch (const narclog::ErrorException& e)
    {
        NARCLOG_ERROR("Error: " + std::string(e.what()));
        engineShutdown();
        return -1;
    }
    catch (const std::exception& e)
    {
        NARCLOG_ERROR("Exception: " + std::string(e.what()));
        engineShutdown();
        return -1;
    }

    engineShutdown();
    return 0;
}

#endif

#endif