#pragma once

#define NARCLOG_FATAL(message) throw narclog::FatalException(message)
#define NARCLOG_ERROR(message) throw narclog::ErrorException(message)
#define NARCLOG_WARNING(message) narclog::log(WARNING, message)
#define NARCLOG_INFO(message) narclog::log(INFO, message)
#define NARCLOG_DEBUG(message) narclog::log(DEBUG, message)

#include "pch.h"

#include "Core.h"

#include "keywords/KeyWords.h"
#include "keywords/LogLevel.h"

#include "exceptions/ErrorException.h"
#include "exceptions/FatalException.h"

#include "exceptions/ExceptionHandlerBuilder.h"
#include "exceptions/MethodExceptionHandler.h"

namespace narclog
{
    NARCLOG_API void createLogger();
    NARCLOG_API void destroyLogger();

    template <LogConcept TMsg>
    NARCLOG_API void log(LogLevel level, TMsg message);
}
