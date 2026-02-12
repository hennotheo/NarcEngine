//
// Created by theoh on 26/02/2025.
//
#include "NarcLog.h"

#include <csignal>

namespace {
    void display_backtrace()
    {
        //TODO: Display backtrace here.
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

    NO_RETURN void handle_signal(const int signal)
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
