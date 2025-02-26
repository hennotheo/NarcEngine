//
// Created by theoh on 26/02/2025.
//

#include "NarcLogger.h"

namespace narclog
{
    NarcLogger* g_logger = nullptr;

    NarcLogger::NarcLogger()
    {
    }

    NarcLogger::~NarcLogger()
    {
    }

    void NarcLogger::log(const char* message)
    {
        std::cout << message << std::endl;
    }

    void createLogger()
    {
        if (g_logger != nullptr)
        {
            throw std::runtime_error("Logger already created.");
        }

        g_logger = new NarcLogger();
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
} // narclog
