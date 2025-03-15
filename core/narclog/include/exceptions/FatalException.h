//
// Created by theoh on 27/02/2025.
//

#pragma once

namespace narclog
{
    class NARCLOG_API FatalException final : public std::runtime_error
    {
        friend class MethodExceptionHandler;

    public:
        explicit FatalException(const std::string& message)
            : runtime_error(message)
        {
        }

    private:
        const char* m_name = nullptr;
    };
}
