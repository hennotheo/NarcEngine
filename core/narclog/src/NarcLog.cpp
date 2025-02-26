//
// Created by theoh on 26/02/2025.
//
#include "NarcLog.h"

#include "Logger.h"

namespace narclog
{
    Logger* g_logger = nullptr;

    void createLogger()
    {
        if (g_logger != nullptr)
        {
            throw std::runtime_error("Logger already created.");
        }

        g_logger = new Logger();
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

    void log(LogLevel level, const char* message)
    {
        g_logger->log(level, message);
    }
} // narclog
