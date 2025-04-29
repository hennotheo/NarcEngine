//
// Created by theoh on 26/02/2025.
//
#include "NarcLog.h"

#include "Logger.h"

#include "exceptions/ErrorException.h"
#include "exceptions/FatalException.h"

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#include "platform/windows/WindowsLogger.h"
#define CREATE_LOGGER new WindowsLogger

#else
#error Unsupported platform.

#endif

namespace narclog {
    Logger* g_logger = nullptr;

    void terminate()
    {
        try
        {
            if (std::current_exception())
            {
                auto exceptionPtr = std::current_exception();
                try
                {
                    if (exceptionPtr)
                    {
                        std::rethrow_exception(exceptionPtr);
                    }
                }
                catch (const narclog::FatalException& e)
                {
                    std::cerr << "Fatal exception: " << e.what() << std::endl;
                }
                catch (const narclog::ErrorException& e)
                {
                    std::cerr << "Error exception: " << e.what() << std::endl;
                    return;
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Unhandled exception: " << e.what() << std::endl;
                }
                catch (...)
                {
                    std::cerr << "Unhandled unknown exception." << std::endl;
                }
            }
        }
        catch (...)
        {
            std::cerr << "Error while handling exception during termination." << std::endl;
        }

        if (g_logger != nullptr)
        {
            destroyLogger();
        }

        std::abort();
    }

    void createLogger()
    {
        if (g_logger != nullptr)
        {
            throw std::runtime_error("Logger already created.");
        }

        g_logger = CREATE_LOGGER();
        std::set_terminate(terminate);
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

    // template<LogConcept TMsg>
    template <typename... Args>
    void log(LogLevel level, const Args&... args)
    {
        std::ostringstream oss;
        ((oss << args), ...); // Fold expression to concatenate all arguments
        std::string message = oss.str();

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
        // if constexpr (MessageConcept<TMsg>)
        // {
        //     g_logger->log(level, message);
        //     return;
        // }

        // if constexpr (ArithmeticConcept<TMsg>)
        // {
        //     g_logger->log(level, message);
        // }

        // NARCLOG_FATAL("Message type " + std::string(typeid(message).name()) + " not supported.");
#endif 
    }

    // template void log<const char*>(LogLevel, const char*);
    // template void log<const std::string&>(LogLevel, const std::string&);
    // template void log<std::string>(LogLevel, std::string);
    // template void log<std::string&>(LogLevel, std::string&);
    // template void log<size_t>(LogLevel, size_t);
    // template void log<uint32_t>(LogLevel, uint32_t);
    // template void log<uint16_t>(LogLevel, uint16_t);
    // template void log<bool>(LogLevel, bool);
}