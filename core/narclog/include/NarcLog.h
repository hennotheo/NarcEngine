#pragma once

#include "Core.h"

#include "LogLevel.h"

namespace narclog
{
    NARC_LOG_API void createLogger();
    NARC_LOG_API void destroyLogger();

    NARC_LOG_API void log(LogLevel level, const char* message);
}