//
// Created by theoh on 28/02/2025.
//

#pragma once

namespace narclog
{
    class FileLogger final
    {
    public:
        FileLogger();
        ~FileLogger();

        void addLine(const std::string& line);
        void writeFile();

    private:
        std::string m_fileName;
        std::vector<std::string> m_lines;
    };
} // narclog
