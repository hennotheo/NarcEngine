//
// Created by theoh on 27/02/2025.
//

#pragma once

#include <NarcLog.h>

namespace narclog
{
    class NARC_LOG_API ErrorException final : public std::runtime_error
    {
    public:
        explicit ErrorException(const std::string& message): runtime_error(message)
        {
        }
    };
}
