//
// Created by theohenno on 10/9/25.
//

#pragma once

namespace narc_log {
     class NarcEngineException : public std::runtime_error {
    public:
        explicit NarcEngineException(const std::string &message) : runtime_error(message) {
        }
    };
} // narc_log
