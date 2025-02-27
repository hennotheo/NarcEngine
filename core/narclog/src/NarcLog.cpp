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

    template <MessageConcept TMsg>
    void log(LogLevel level, TMsg message)
    {
        g_logger->log(level, message);
    }

    template void log<const char*>(LogLevel, const char*);
    template void log<std::string>(LogLevel, std::string);
    template void log<std::string&>(LogLevel, std::string&);
    template void log<const std::string&>(LogLevel, const std::string&);
    template void log<std::string*>(LogLevel, std::string*);
    template void log<const std::string*>(LogLevel, const std::string*);
} // narclog
