//
// Created by theoh on 26/02/2025.
//

#include "Logger.h"

#include <keywords/ConsoleStyle.h>

#include "FileLogger.h"

namespace narclog {
    Logger::Logger()
    {
        m_fileLogger = std::make_unique<FileLogger>();
    }

    Logger::~Logger()
    {
        m_fileLogger->writeFile(false);
    }

    void Logger::log(LogLevel level, const std::string& message)
    {
#ifndef NARC_BUILD_DEBUG
        if (level == DEBUG)
            return;
#endif

        const std::string date = currentDateTime("%Y-%m-%d %H:%M:%S");
        const std::string prefix = prefixForLevel(level);
        const std::string color = colorForLevel(level);
        std::cout << CONSOLE_MESSAGE_FORMATER(color, prefix, message, date) << std::endl;

        m_fileLogger->addLine(LOG_MESSAGE_FORMATER(prefix, message, date));

        if (level == FATAL)
        {
            m_fileLogger->writeFile(true);
        }
    }

    std::string Logger::currentDateTime(const char* format)
    {
        std::time_t now = std::time(nullptr);
        char buf[100];
        std::strftime(buf, sizeof(buf), format, std::localtime(&now));
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
