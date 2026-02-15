//
// Created by theohenno on 10/9/25.
//

#pragma once

namespace narc_log {
    /**
     * Exception class for handling errors specific to the NarcEngine.
     * Inherits from std::runtime_error to provide standard exception functionality.
     */
    class RuntimeError : public std::runtime_error {
    public:
        explicit RuntimeError(const std::string &message) : std::runtime_error(message) {
        }

        ~RuntimeError() noexcept override = default;
    };
} // narc_log
