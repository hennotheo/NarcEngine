//
// Created by theoh on 25/05/2025.
//

#pragma once

#if __cplusplus >= 202002L && __has_include(<format>)
#define ENGINE_HAS_STD_FORMAT
#include <format>
#else
    #include <sstream>
    #include <iomanip>
#endif

#ifdef ENGINE_HAS_STD_FORMAT

template<typename FmtArg>
constexpr FmtArg GET_FORMAT_EXPR(const FmtArg& fmt)
{
    return fmt;
}

template<typename... Args>
std::string GET_FORMAT_EXPR(std::format_string<Args...> fmt, Args&&... rest)
{
    return std::format(fmt, std::forward<Args>(rest)...);
}

#define NARC_FIRST_ARG(first, ...) first

#define NARC_FORMAT(...) GET_FORMAT_EXPR(__VA_ARGS__)
#define NARC_CONCAT(a, b) std::format("{}{}", a, b)

#else

    template<typename... Args>
    std::string engineFormat(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << args);
        return oss.str();
    }
#define NARC_FORMAT(...) engineFormat(__VA_ARGS__)
#define NARC_CONCAT(a, b) engineFormat(a, b)

#endif
