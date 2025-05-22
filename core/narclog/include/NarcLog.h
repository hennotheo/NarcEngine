#pragma once

#include "pch.h"

#include "Core.h"

#define NARCLOG_FATAL(message) throw narclog::FatalException(std::string(message))
#define NARCLOG_ERROR(message) throw narclog::ErrorException(std::string(message))
#define NARCLOG_WARNING(message) narclog::logString(WARNING, std::string(message))
#define NARCLOG_INFO(message) narclog::logString(INFO, std::string(message))
#define NARCLOG_DEBUG(message) narclog::logString(DEBUG, std::string(message))

#define NARCLOG_INIT(callback) narclog::createLogger(); \
    narclog::setSafeCloseCallback(callback)

#include "keywords/LogLevel.h"

#include "exceptions/ErrorException.h"
#include "exceptions/FatalException.h"

namespace narclog
{
    NARCLOG_API void createLogger();
    NARCLOG_API void destroyLogger();

    NARCLOG_API void setSafeCloseCallback(std::function<void()> callback);

    NARCLOG_API void logString(LogLevel level, const std::string& message);

    template <typename... Args>
    NARCLOG_API inline void log(LogLevel level, const Args&...args)
    {
        std::ostringstream oss;
        (oss << ... << args);
        logString(level, oss.str());
    }
}