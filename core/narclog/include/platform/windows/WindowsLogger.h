//
// Created by theoh on 26/02/2025.
//

#pragma once

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#include "Logger.h"

namespace narclog
{
    class WindowsLogger final : public Logger
    {
    public:
        WindowsLogger();
        ~WindowsLogger() override;
    };
} // narclog

#endif
