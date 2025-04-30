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
    std::mutex loggingMutex;
    std::mutex terminateMutex;
    Logger* g_logger = nullptr;

    void handleTerminate()
    {
        std::lock_guard<std::mutex> lock(terminateMutex);
        if (g_logger == nullptr)
        {
            std::cout << "Logger not created." << std::endl;
            return;
        }

        g_logger->onTerminate();

        destroyLogger();

        std::abort();
    }

    void setSafeCloseCallback(std::function<void()> callback)
    {
        g_logger->setSafeCloseCallback(callback);
    }

    void createLogger()
    {
        if (g_logger != nullptr)
        {
            throw std::runtime_error("Logger already created.");
        }

        try
        {
            g_logger = CREATE_LOGGER();
            std::set_terminate(handleTerminate);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error("Failed to create logger: " + std::string(e.what()));
        }
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

    void logString(LogLevel level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(loggingMutex);

        if (level == LogLevel::FATAL)
        {
            throw narclog::FatalException(message);
        }

        if (level == LogLevel::ERROR)
        {
            throw narclog::ErrorException(message);
        }

#if !defined(NARC_TEST_BUILD) || (defined(NARC_TEST_BUILD) && defined(NARCLOG_BUILD_DLL))
        if (g_logger == nullptr)
        {
            std::cout << "Logger not created : " << message << std::endl;
            throw std::runtime_error("Logger not created.");
        }

        g_logger->log(level, message);
#endif
    }
}