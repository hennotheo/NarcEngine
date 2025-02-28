//
// Created by theoh on 26/02/2025.
//

#pragma once

#include <memory>
#include <NarcLog.h>

namespace narclog {
    class FileLogger;

    class Logger
    {
    public:
        Logger();
        virtual ~Logger();

        void log(LogLevel level, const std::string& message);

    private:
        std::unique_ptr<FileLogger> m_fileLogger;

        static std::string currentDateTime();
        static std::string prefixForLevel(LogLevel level);
        static std::string colorForLevel(LogLevel level);
    };
}
