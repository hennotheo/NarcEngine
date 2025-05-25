//
// Created by theoh on 28/02/2025.
//

#include "FileLogger.h"

#include <NarcLog.h>

#include "Logger.h"
#include "keywords/LogLevel.h"

namespace narclog {
    const char* g_path = "logs/";
    const char* g_extension = ".txt";

    FileLogger::FileLogger()
    {
        m_date = Logger::currentDateTime("%Y-%m-%d_%H-%M-%S");

#ifdef NARC_BUILD_DEBUG
        m_alwaysWriteLogs = true;
#endif
    }

    FileLogger::~FileLogger()
    {
    }

    void FileLogger::addLine(const std::string& line)
    {
        m_lines.push_back(line);
    }

    void FileLogger::writeFile(bool crashing)
    {
        const bool mustWriteLogs = m_alwaysWriteLogs || crashing;
        if (m_alreadyWritten || !mustWriteLogs)
        {
            return;
        }

        m_alreadyWritten = true;

        const std::string filename = g_path + m_date + g_extension;
        if (!std::filesystem::exists(g_path))
        {
            std::filesystem::create_directories(g_path);
        }

        std::ofstream file(filename);
        if (!file.is_open())
        {
            NARCLOG_ERROR("Failed to open file: {}", filename);
        }

        for (const std::string& line: m_lines)
        {
            file << line << std::endl;
        }

        file.close();
    }
} // narclog
