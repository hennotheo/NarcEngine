//
// Created by theoh on 26/02/2025.
//

#include "Logger.h"

#include <keywords/ConsoleStyle.h>

namespace narclog
{
    template void Logger::log<const char*>(LogLevel, const char*);
    template void Logger::log<std::string>(LogLevel, std::string);
    template void Logger::log<std::string&>(LogLevel, std::string&);
    template void Logger::log<const std::string&>(LogLevel, const std::string&);
    template void Logger::log<std::string*>(LogLevel, std::string*);
    template void Logger::log<const std::string*>(LogLevel, const std::string*);

    Logger::Logger()
    {
    }

    Logger::~Logger()
    {
    }

    template <MessageConcept TMsg>
    void Logger::log(LogLevel level, TMsg message)
    {
        #ifndef NARC_BUILD_DEBUG
        if (level == DEBUG)
            return;
        #endif

        const std::string date = currentDateTime();
        std::cout << CONSOLE_MESSAGE_WITH_DATE_FORMATER(colorForLevel(level), prefixForLevel(level), message, date)
            << std::endl;
    }

    std::string Logger::currentDateTime()
    {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return buf;
    }

    std::string Logger::prefixForLevel(LogLevel level)
    {
        switch (level)
        {
        case FATAL: return CONSOLE_MESSAGE_PREFIX_FATAL;
        case ERROR: return CONSOLE_MESSAGE_PREFIX_ERROR;
        case WARNING: return CONSOLE_MESSAGE_PREFIX_WARNING;
        case INFO: return CONSOLE_MESSAGE_PREFIX_INFO;
        case DEBUG: return CONSOLE_MESSAGE_PREFIX_DEBUG;
        default: return "UNKNOWN";
        }
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
