//
// Created by theoh on 26/02/2025.
//

#pragma once

namespace narclog
{
    class NarcLogger
    {
    public:
        NarcLogger();
        ~NarcLogger();

        void log(const char* message);
    };

    NARC_LOG_API void createLogger();
    NARC_LOG_API void destroyLogger();
}
