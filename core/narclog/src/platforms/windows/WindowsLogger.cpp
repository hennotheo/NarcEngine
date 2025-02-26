//
// Created by theoh on 26/02/2025.
//

#ifdef NARC_ENGINE_PLATFORM_WINDOWS

#include "platform/windows/WindowsLogger.h"

#include <windows.h>

namespace narclog
{
    WindowsLogger::WindowsLogger()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    WindowsLogger::~WindowsLogger()
    {
    }
}

#endif
