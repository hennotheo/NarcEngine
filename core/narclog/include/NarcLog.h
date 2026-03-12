#pragma once

#include "Core.h"
#include "pch.h"

#include "exceptions/LogicError.h"
#include "exceptions/NotImplementedError.h"
#include "exceptions/RuntimeError.h"

#include "keywords/LogLevel.h"

#define NARC_ERROR_LOGIC(...) throw narc_log::LogicError(narc_log::format(__VA_ARGS__))
#define NARC_ERROR_RUNTIME(...) throw narc_log::RuntimeError(narc_log::format(__VA_ARGS__))
#define NARC_ERROR_NOT_IMPLEMENTED(...) throw narc_log::NotImplementedError(narc_log::format(__VA_ARGS__))

#define NARC_LOG_DEBUG(...) narc_log::log(DEBUG, __VA_ARGS__)
#define NARC_LOG_INFO(...) narc_log::log(INFO, __VA_ARGS__)
#define NARC_LOG_WARNING(...) narc_log::log(WARNING, __VA_ARGS__)
#define NARC_LOG_ERROR(...) narc_log::log(ERROR, __VA_ARGS__)
#define NARC_LOG_FATAL(...) narc_log::log(FATAL, __VA_ARGS__)

#define NARC_GUARD_RAW_PTR(ptr, errorMsg)                                                                                                                                                                                                        \
    if (ptr == nullptr)                                                                                                                                  \
    {                                                                                                                                                \
        NARC_ERROR_RUNTIME(errorMsg);                                                                                                                \
    }

namespace narc_log {
    [[maybe_unused]] NARC_LOG_API void init_signal_handling();

    template<typename... Args>
    [[maybe_unused]] NARC_LOG_API void log(const LogLevel& level, spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        switch (level)
        {
            case DEBUG: spdlog::debug(fmt, std::forward<Args>(args)...);
                break;
            case INFO: spdlog::info(fmt, std::forward<Args>(args)...);
                break;
            case WARNING: spdlog::warn(fmt, std::forward<Args>(args)...);
                break;
            case ERROR: spdlog::error(fmt, std::forward<Args>(args)...);
                break;
            case FATAL: spdlog::critical(fmt, std::forward<Args>(args)...);
                break;
        }
    }

    template<typename... Args>
    [[maybe_unused]] NARC_LOG_API std::string format(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        return fmt::format(fmt, std::forward<Args>(args)...);
    }

    [[maybe_unused]] NARC_LOG_API inline std::string format(const std::string& message) { return message; }
    [[maybe_unused]] NARC_LOG_API inline std::string format(const char* message) { return message; }
} // namespace narc_log
