//
// Created by theoh on 27/02/2025.
//

#pragma once

/**
 * \def NARCLOG_EXECUTE_FATAL_SAFE
 * \brief Macro to execute a function safely, handling all non-fatal exceptions as error logs and fatal exceptions.
 * \param function The function to be executed. */
#define NARCLOG_EXECUTE_FATAL_SAFE(function) narclog::ExceptionHandlerBuilder() \
            .bind([] { function; }) \
            ->handleAllNonFatalExceptionAsErrorLog() \
            ->handleFatal() \
            ->create() \
            .invoke();

#define NARCLOG_EXECUTE_FINALLY_FATAL_SAFE(function, finallyCallback) narclog::ExceptionHandlerBuilder() \
            .bind([] { function; }) \
            ->bindFinally([] { finallyCallback; }) \
            ->handleAllNonFatalExceptionAsFatal() \
            ->handleFatal() \
            ->create() \
            .invoke();

namespace narclog
{
    class MethodExceptionHandler;

    class NARC_LOG_API ExceptionHandlerBuilder final
    {
    public:
        ExceptionHandlerBuilder* bind(const std::function<void()>& function);
        ExceptionHandlerBuilder* bindFinally(const std::function<void()>& finallyCallback);

        ExceptionHandlerBuilder* handleAllNonFatalExceptionAsFatal();
        ExceptionHandlerBuilder* handleAllNonFatalExceptionAsErrorLog();
        ExceptionHandlerBuilder* rethrowFatal();
        ExceptionHandlerBuilder* handleFatal();

        [[nodiscard]] MethodExceptionHandler create() const;

    private:
        std::function<void()> m_function;
        std::function<void()> m_finallyCallback;

        bool m_handleAllNonFatalExceptionAsFatal = false;
        bool m_rethrowFatal = true;
    };
} // narclog
