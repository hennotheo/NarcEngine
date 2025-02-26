//
// Created by theoh on 26/02/2025.
//

#include "Logger.h"

namespace narclog
{
    Logger::Logger()
    {
    }

    Logger::~Logger()
    {
    }

    void Logger::log(LogLevel level, const char* message)
    {
#ifdef
        std::cout << level << message << std::endl;
    }
} // narclog
