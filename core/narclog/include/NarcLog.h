#pragma once

#include "pch.h"
#include "Core.h"

#include "LogLevel.h"

#include "exceptions/FatalException.h"
#include "exceptions/ErrorException.h"

#define NARCLOG_FATAL(message) narclog::ErrorException(message)
#define NARCLOG_ERROR(message) narclog::ErrorException(message)
#define NARCLOG_WARNING(message) narclog::log(WARNING, message)
#define NARCLOG_INFO(message) narclog::log(INFO, message)
#define NARCLOG_DEBUG(message) narclog::log(DEBUG, message)

namespace narclog
{
    NARC_LOG_API void createLogger();
    NARC_LOG_API void destroyLogger();

    NARC_LOG_API void executeWithExceptionHandling(const std::function<void()>& function);

    NARC_LOG_API void log(LogLevel level, const char* message);
}