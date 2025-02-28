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
        const std::string date = "";
        m_fileName = "log" + date + ".txt";
    }

    FileLogger::~FileLogger()
    {
    }

    void FileLogger::addLine(const std::string& line)
    {
        m_lines.push_back(line);
    }

    void FileLogger::writeFile()
    {
        std::ofstream file(m_fileName);
        if (!file.is_open())
        {
            NARCLOG_ERROR("Failed to open file: " + m_fileName);
        }

        for (const std::string& line: m_lines)
        {
            file << line << std::endl;
        }
        NARCLOG_DEBUG("WRITING FILE");

        file.close();
    }
} // narclog
