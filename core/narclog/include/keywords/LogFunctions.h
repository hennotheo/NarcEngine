//
// Created by theoh on 25/05/2025.
//

#pragma once

#include "LogLevel.h"

namespace narclog
{
    NARCLOG_API void createLogger();
    NARCLOG_API void destroyLogger();

    NARCLOG_API void setSafeCloseCallback(std::function<void()> callback);

    NARCLOG_API void logString(LogLevel level, const std::string& message);
}

#define NARCLOG_FATAL(...) throw narclog::FatalException(NARC_FORMAT(__VA_ARGS__))
#define NARCLOG_ERROR(...) throw narclog::ErrorException(NARC_FORMAT(__VA_ARGS__))
#define NARCLOG_WARNING(...) narclog::logString(WARNING, NARC_FORMAT(__VA_ARGS__))
#define NARCLOG_INFO(...) narclog::logString(INFO, NARC_FORMAT(__VA_ARGS__))
#define NARCLOG_DEBUG(...) narclog::logString(DEBUG, NARC_FORMAT(__VA_ARGS__))

#define NARCLOG_INIT(callback) narclog::createLogger(); \
narclog::setSafeCloseCallback(callback)