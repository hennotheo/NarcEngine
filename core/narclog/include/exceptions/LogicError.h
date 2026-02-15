//
// Created by theohenno on 10/9/25.
//

#pragma once

namespace narc_log {
    /**
     * Exception class for handling logic errors specific to the NarcEngine.
     * Inherits from std::logic_error to provide standard exception functionality.
     */
    class LogicError : public std::logic_error
    {
    public:
        explicit LogicError(const std::string& message) : std::logic_error(message) {}

        ~LogicError() noexcept override = default;
    };
} // namespace narc_log
