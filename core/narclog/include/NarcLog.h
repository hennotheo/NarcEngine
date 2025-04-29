#pragma once

#include "pch.h"

#include "Core.h"

#define NARCLOG_FATAL(message) throw narclog::FatalException(message)
#define NARCLOG_ERROR(message) throw narclog::ErrorException(message)
#define NARCLOG_WARNING(message) narclog::log(WARNING, message)
#define NARCLOG_INFO(message) narclog::log(INFO, message)
#define NARCLOG_DEBUG(message) narclog::log(DEBUG, message)

#define NARCLOG_INIT() narclog::createLogger(); \
    std::set_terminate(narclog::handleTerminate) \

#define NARCLOG_INIT_WITH_CALLBACK(callback) NARCLOG_INIT(); \
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
    NARCLOG_API void handleTerminate();

    template <typename... Args>
    NARCLOG_API inline void log(LogLevel level, const Args&...args)
    {
        std::ostringstream oss;
        (oss << ... << args);
        logString(level, oss.str());
    }
}