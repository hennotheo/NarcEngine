//
// Created by theoh on 28/02/2025.
//

#include "FileLogger.h"

#include <fstream>

#include "NarcLog.h"
#include "keywords/LogLevel.h"

namespace narclog {
    FileLogger::FileLogger()
    {
        NARCLOG_WARNING("Date non implemented.");
        const std::string date = "";
        m_fileName = "log" + date + ".txt";
        m_lines = std::vector<std::string>();
    }

    FileLogger::~FileLogger()
    {
    }

    void FileLogger::addLine(const std::string& line)
    {
        NARCLOG_DEBUG(m_lines.size());
        m_lines.push_back(line);
    }

    void FileLogger::writeFile()
    {
        std::ofstream file(m_fileName);
        if (!file.is_open())
        {
            NARCLOG_ERROR("Failed to open file: " + m_fileName);
        }

        for (const std::string& line : m_lines)
        {
            file << line << std::endl;
        }

        file.close();
    }
} // narclog