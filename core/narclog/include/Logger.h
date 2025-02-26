//
// Created by theoh on 26/02/2025.
//

#pragma once

#include <NarcLog.h>

namespace narclog
{
    class Logger
    {
    public:
        Logger();
        virtual ~Logger();

        void log(LogLevel level, const char* message);

    private:
        static std::string colorForLevel(LogLevel level);
    };
}
