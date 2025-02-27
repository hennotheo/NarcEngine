//
// Created by theoh on 27/02/2025.
//

#include "exceptions/MethodExceptionHandler.h"

namespace narclog
{
    int MethodExceptionHandler::invoke() const
    {
        try
        {
            m_function();
            finally();
            return EXIT_SUCCESS;
        }
        catch (const FatalException& e)
        {
            fatalExceptionHandler(e);
        }
        catch (const ErrorException& e)
        {
            errorExceptionHandler(e);
        }
        catch (const std::exception& e)
        {
            if (m_handleAllNonFatalExceptionAsFatal)
            {
                fatalExceptionHandler(e);
            }
            else
            {
                errorExceptionHandler(e);
            }
        }

        return EXIT_FAILURE;
    }

    MethodExceptionHandler::MethodExceptionHandler(const std::function<void()>& function,
                                                   const std::function<void()>& finallyCallback) :
        m_function(function),
        m_finallyCallback(finallyCallback)
    {
    }

    void MethodExceptionHandler::fatalExceptionHandler(const std::exception& e) const
    {
        finally();

        if (m_rethrowFatal)
        {
            throw NARCLOG_FATAL(e.what());
        }
        else
        {
            log(FATAL, e.what());
        }
    }

    void MethodExceptionHandler::errorExceptionHandler(const std::exception& e) const
    {
        finally();
        log(ERROR, e.what());
    }

    void MethodExceptionHandler::finally() const
    {
        if (m_finallyCallback)
            m_finallyCallback();
    }

    MethodExceptionHandler::~MethodExceptionHandler()
    {
    }
} // narclog
