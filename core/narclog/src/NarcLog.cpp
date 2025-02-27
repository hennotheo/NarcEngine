//
// Created by theoh on 26/02/2025.
//
#include "NarcLog.h"

#include "Logger.h"

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#include "platform/windows/WindowsLogger.h"
#define CREATE_LOGGER new WindowsLogger

#else
#error Unsupported platform.

#endif

namespace narclog
{
    Logger* g_logger = nullptr;

    void createLogger()
    {
        if (g_logger != nullptr)
        {
            throw std::runtime_error("Logger already created.");
        }

        g_logger = CREATE_LOGGER();
    }

    void destroyLogger()
    {
        if (g_logger == nullptr)
        {
            throw std::runtime_error("Logger already destroyed.");
        }

        delete g_logger;
        g_logger = nullptr;
    }

    void executeWithExceptionHandling(const std::function<void()>& function)
    {
        try
        {
            function();
        }
        catch (const std::exception& e)
        {
            
        }
        // catch (const narclog::FatalException& e)
        // {
        //     app->stop();
        //     NARCLOG_FATAL(e.what());
        //     std::cin.get();
        //
        //     delete app;
        //     narclog::destroyLogger();
        //     return EXIT_FAILURE;
        // }
        // catch (const std::exception& e)
        // {
        //     app->stop();
        //     std::cin.get();
        //
        //     delete app;
        //     narclog::destroyLogger();
        //     return EXIT_FAILURE;
        // }
    }

    void log(LogLevel level, const char* message)
    {
        g_logger->log(level, message);
    }
} // narclog
