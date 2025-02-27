//
// Created by theoh on 27/02/2025.
//

#include "exceptions/ExceptionHandlerBuilder.h"

#include <NarcLog.h>

namespace narclog
{
    ExceptionHandlerBuilder* ExceptionHandlerBuilder::bind(const std::function<void()>& function)
    {
        m_function = function;
        return this;
    }

    ExceptionHandlerBuilder* ExceptionHandlerBuilder::bindFinally(const std::function<void()>& finallyCallback)
    {
        m_finallyCallback = finallyCallback;
        return this;
    }

    ExceptionHandlerBuilder* ExceptionHandlerBuilder::handleAllNonFatalExceptionAsFatal()
    {
        m_handleAllNonFatalExceptionAsFatal = true;
        return this;
    }

    ExceptionHandlerBuilder* ExceptionHandlerBuilder::handleAllNonFatalExceptionAsErrorLog()
    {
        m_handleAllNonFatalExceptionAsFatal = false;
        return this;
    }

    ExceptionHandlerBuilder* ExceptionHandlerBuilder::rethrowFatal()
    {
        m_rethrowFatal = true;
        return this;
    }

    ExceptionHandlerBuilder* ExceptionHandlerBuilder::handleFatal()
    {
        m_rethrowFatal = false;
        return this;
    }

    MethodExceptionHandler ExceptionHandlerBuilder::create() const
    {
        MethodExceptionHandler handler(m_function, m_finallyCallback);
        handler.m_handleAllNonFatalExceptionAsFatal = m_handleAllNonFatalExceptionAsFatal;
        handler.m_rethrowFatal = m_rethrowFatal;
        return handler;
    }
} // narclog
