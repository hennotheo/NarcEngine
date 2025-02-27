//
// Created by theoh on 27/02/2025.
//

#pragma once

#include <NarcLog.h>

namespace narclog
{
    class NARC_LOG_API MethodExceptionHandler final
    {
        friend class ExceptionHandlerBuilder;
    public:
        ~MethodExceptionHandler();

        int invoke() const;

    private:
        explicit MethodExceptionHandler(const std::function<void()>& function, const std::function<void()>& finallyCallback);

        const std::function<void()> m_function;
        const std::function<void()> m_finallyCallback;

        bool m_handleAllNonFatalExceptionAsFatal = false;
        bool m_rethrowFatal = false;

        void fatalExceptionHandler(const std::exception& e) const;
        void errorExceptionHandler(const std::exception& e) const;

        void finally() const;
    };
} // narclog