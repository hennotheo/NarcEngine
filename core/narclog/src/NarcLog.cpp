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
    Logger *g_logger = nullptr;

    void terminate()
    {
        if (g_logger == nullptr)
        {
            std::cout << "Logger not created." << std::endl;
            return;
        }

        std::exception_ptr currentException = std::current_exception();
        if (currentException)
        {
            try
            {
                // Relancer l'exception pour la capturer
                std::rethrow_exception(currentException);
            }
            catch (const std::exception &e)
            {
                // Afficher les informations sur l'exception
                std::cerr << "Exception non interceptée : " << e.what() << std::endl;
            }
            catch (...)
            {
                // Gérer les exceptions non dérivées de std::exception
                std::cerr << "Exception non interceptée : type inconnu" << std::endl;
            }
        }
        else
        {
            std::cerr << "Aucune exception courante au moment de terminate" << std::endl;
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

    void logString(LogLevel level, const std::string &message)
    {
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