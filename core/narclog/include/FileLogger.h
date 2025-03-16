//
// Created by theoh on 28/02/2025.
//

#pragma once

namespace narclog {
    class FileLogger final
    {
    public:
        FileLogger();
        ~FileLogger();

        void addLine(const std::string& line);
        void writeFile(bool crashing);

    private:
        std::string m_date;
        std::vector<std::string> m_lines;

        bool m_alwaysWriteLogs = false;
        bool m_alreadyWritten = false;
    };
} // narclog
