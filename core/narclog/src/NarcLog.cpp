//
// Created by theoh on 26/02/2025.
//
#include "NarcLog.h"

#include "Logger.h"

#include "exceptions/ErrorException.h"
#include "exceptions/FatalException.h"

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#include "platform/windows/WindowsLogger.h"
#define CREATE_LOGGER new narclog::WindowsLogger

#else
#error Unsupported platform.

#endif

namespace narclog
{
    std::mutex mutex;
    Logger* g_logger = nullptr;

    void setSafeCloseCallback(std::function<void()> callback)
    {
        g_logger->setSafeCloseCallback(callback);
    }

    void handleTerminate()
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (g_logger == nullptr)
        {
            std::cout << "Logger not created." << std::endl;
            return;
        }

        g_logger->onTerminate();

        destroyLogger();

        std::abort();
    }

    void createLogger()
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (g_logger != nullptr)
        {
            throw std::runtime_error("Logger already created.");
        }

        g_logger = CREATE_LOGGER();
    }

    void destroyLogger()
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (g_logger == nullptr)
        {
            throw std::runtime_error("Logger already destroyed.");
        }

        delete g_logger;
        g_logger = nullptr;
    }

    void logString(LogLevel level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (level == LogLevel::FATAL)
        {
            throw narclog::FatalException(message);
        }

        if (level == LogLevel::ERROR)
        {
            throw narclog::ErrorException(message);
        }

#ifndef NARC_TEST_BUILD
        if (g_logger == nullptr)
        {
            std::cout << "Logger not created : " << message << std::endl;
            throw std::runtime_error("Logger not created.");
        }

        g_logger->log(level, message);
#endif
    }
}