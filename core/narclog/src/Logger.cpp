//
// Created by theoh on 26/02/2025.
//

#include "Logger.h"

#include "ConsoleStyle.h"

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
#ifndef NARC_BUILD_DEBUG
        if (level == DEBUG)
            return;
#endif

        std::cout << colorForLevel(level) << message << CONSOLE_TEXT_COLOR_DEFAULT << std::endl;
    }

    std::string Logger::colorForLevel(LogLevel level)
    {
        switch (level)
        {
        case FATAL: return CONSOLE_TEXT_COLOR_RED;
        case ERROR: return CONSOLE_TEXT_COLOR_RED;
        case WARNING: return CONSOLE_TEXT_COLOR_YELLOW;
        case INFO: return CONSOLE_TEXT_COLOR_WHITE;
        case DEBUG: return CONSOLE_TEXT_COLOR_CYAN;
        default: return CONSOLE_TEXT_COLOR_DEFAULT;
        }
    }
} // narclog
