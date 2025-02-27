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

        template <MessageConcept TMsg>
        void log(LogLevel level, TMsg message);

    private:
        static std::string currentDateTime();
        static std::string prefixForLevel(LogLevel level);
        static std::string colorForLevel(LogLevel level);
    };
}
