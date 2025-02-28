//
// Created by theoh on 26/02/2025.
//

#pragma once

#include <NarcLog.h>

#include "FileLogger.h"

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
        FileLogger m_fileLogger;

        static std::string currentDateTime();
        static std::string prefixForLevel(LogLevel level);
        static std::string colorForLevel(LogLevel level);
    };
}
