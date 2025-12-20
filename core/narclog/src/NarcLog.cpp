//
// Created by theoh on 26/02/2025.
//
#include "NarcLog.h"

#include <csignal>
#include <execinfo.h>

namespace {
    void display_backtrace()
    {
        // Get void*'s for all entries on the stack
        void* array[20];
        const size_t size = backtrace(array, 20);
        char** symbols = backtrace_symbols(array, static_cast<int>(size));

        // Calculate total size needed for the output string
        size_t total = 0;
        for (size_t i = 0; i < size; ++i)
        {
            total += strlen(symbols[i]) + 1;
        }

        // Create a single string to hold the entire backtrace
        std::string out;
        out.reserve(total);
        for (size_t i = 0; i < size; ++i)
        {
            out.append("\n");
            out.append(symbols[i]);
        }

        NARC_LOG_FATAL("Stack trace: {}", out);

        free(symbols);
    }

    void display_signal_log(int signal)
    {
        switch (signal)
        {
            case SIGINT:
                NARC_LOG_FATAL("Caught SIGINT, terminating.");
                break;
            case SIGSEGV:
                NARC_LOG_FATAL("Caught SIGSEGV, segmentation fault.");
                break;
            case SIGABRT:
                NARC_LOG_FATAL("Caught SIGABRT, aborting.");
                break;
            default:
                NARC_LOG_FATAL("Caught unknown signal {}, terminating.", signal);
                break;
        }
    }

    void handle_signal(const int signal)
    {
        display_signal_log(signal);
        display_backtrace();

        std::exit(signal);
    }
}

namespace narc_log {
    void init_signal_handling()
    {
        std::signal(SIGSEGV, handle_signal);
        std::signal(SIGABRT, handle_signal);
        std::signal(SIGFPE, handle_signal);
        std::signal(SIGINT, handle_signal);
    }
}
