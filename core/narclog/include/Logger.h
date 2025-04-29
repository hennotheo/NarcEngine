//
// Created by theoh on 26/02/2025.
//

#pragma once

namespace narclog {
    class FileLogger;

    class Logger
    {
    public:
        Logger();
        virtual ~Logger();

        void log(LogLevel level, const std::string& message);
        void onTerminate();
        void setSafeCloseCallback(std::function<void()> callback);

        static std::string currentDateTime(const char* format);

    private:
        std::unique_ptr<FileLogger> m_fileLogger;
        std::function<void()> m_safeCloseCallback;

        static std::string prefixForLevel(LogLevel level);
        static std::string colorForLevel(LogLevel level);
    };
}
