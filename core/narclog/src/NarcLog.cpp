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

namespace narclog {
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

    template<LogConcept TMsg>
    void log(LogLevel level, TMsg message)
    {
        if (g_logger == nullptr)
        {
            std::cout << "Logger not created : " << message << std::endl;
            throw std::runtime_error("Logger not created.");
        }

        if constexpr (MessageConcept<TMsg>)
        {
            g_logger->log(level, message);
            return;
        }

        if constexpr (ArithmeticConcept<TMsg>)
        {
            g_logger->log(level, std::to_string(message));
        }

        NARCLOG_FATAL("Message type " + std::string(typeid(message).name()) + " not supported.");
    }

    template void log<const char*>(LogLevel, const char*);
    template void log<const std::string&>(LogLevel, const std::string&);
    template void log<std::string>(LogLevel, std::string);
    template void log<std::string&>(LogLevel, std::string&);
    template void log<size_t>(LogLevel, size_t);
    template void log<uint32_t>(LogLevel, uint32_t);
    template void log<uint16_t>(LogLevel, uint16_t);
    template void log<bool>(LogLevel, bool);
}