//
// Created by theoh on 27/02/2025.
//

#pragma once

//TODO Modify to use reflection

#define NARCLOG_HANDLED_METHOD_NAME(handlerName) narclog__method_handler_##handlerName

#define NARCLOG_PREPARE_HANDLER(name) auto NARCLOG_HANDLED_METHOD_NAME(name) = narclog::ExceptionHandlerBuilder() \
            .handleAllNonFatalExceptionAsFatal() \
            ->setName(#name) \
            ->rethrowFatal() \
            ->create()
/**
 * \def NARCLOG_EXECUTE_FATAL_SAFE
 * \brief Macro to execute a function safely, handling all non-fatal exceptions as error logs and fatal exceptions.
 * \param function The function to be executed. */
#define NARCLOG_EXECUTE_FATAL_SAFE(method)  narclog::ExceptionHandlerBuilder() \
            .bind([] { method; }) \
            ->handleAllNonFatalExceptionAsErrorLog() \
            ->handleFatal() \
            ->create() \
            .invoke();

#define NARCLOG_EXECUTE_FINALLY_FATAL_SAFE(method, finallyCallback) narclog::ExceptionHandlerBuilder() \
            .bind([] { method; }) \
            ->bindFinally([] { finallyCallback; }) \
            ->handleAllNonFatalExceptionAsFatal() \
            ->handleFatal() \
            ->create() \
            .invoke();

namespace narclog
{
    class MethodExceptionHandler;

    class NARCLOG_API ExceptionHandlerBuilder final
    {
    public:
        ExceptionHandlerBuilder* bind(const std::function<void()>& function);
        ExceptionHandlerBuilder* bindFinally(const std::function<void()>& finallyCallback);

        ExceptionHandlerBuilder* handleAllNonFatalExceptionAsFatal();
        ExceptionHandlerBuilder* handleAllNonFatalExceptionAsErrorLog();
        ExceptionHandlerBuilder* rethrowFatal();
        ExceptionHandlerBuilder* handleFatal();

        ExceptionHandlerBuilder* setName(const char* name);

        GETTER MethodExceptionHandler create() const;

    private:
        std::function<void()> m_function;
        std::function<void()> m_finallyCallback;

        bool m_handleAllNonFatalExceptionAsFatal = false;
        bool m_rethrowFatal = true;

        const char* m_name = nullptr;
    };
}
