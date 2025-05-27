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

#ifndef NARC_TEST_BUILD

#define NARCLOG_WARNING(...) narclog::logString(WARNING, NARC_FORMAT(__VA_ARGS__))
#define NARCLOG_INFO(...) narclog::logString(INFO, NARC_FORMAT(__VA_ARGS__))
#define NARCLOG_DEBUG(...) narclog::logString(DEBUG, NARC_FORMAT(__VA_ARGS__))

#else

#define NARCLOG_WARNING(...) std::cout << "WARNING: " << NARC_FORMAT(__VA_ARGS__) << std::endl
#define NARCLOG_INFO(...) std::cout << "INFO: " << NARC_FORMAT(__VA_ARGS__) << std::endl
#define NARCLOG_DEBUG(...) std::cout << "DEBUG: " << NARC_FORMAT(__VA_ARGS__) << std::endl

#endif

#define NARCLOG_INIT(callback) \
    narclog::createLogger(); \
    narclog::setSafeCloseCallback(callback)
