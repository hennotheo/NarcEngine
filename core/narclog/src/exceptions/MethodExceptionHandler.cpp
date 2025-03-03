//
// Created by theoh on 27/02/2025.
//

#include "exceptions/MethodExceptionHandler.h"

#include "Logger.h"

namespace narclog {
    int MethodExceptionHandler::invoke()
    {
        return execute(m_function);
    }

    int MethodExceptionHandler::execute(const std::function<void()>& method)
    {
        try
        {
            method();
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
                fatalExceptionHandler(static_cast<const FatalException&>(e));
            }
            else
            {
                errorExceptionHandler(e);
            }
        }

        return EXIT_FAILURE;
    }

    MethodExceptionHandler::MethodExceptionHandler(const std::function<void()>& function,
                                                   const std::function<void()>& finallyCallback) : m_function(function),
                                                                                                   m_finallyCallback(finallyCallback)
    {
    }

    void MethodExceptionHandler::fatalExceptionHandler(const FatalException& e)
    {
        if (m_rethrowFatal)
        {
            finally();

            FatalException newException(e);
            if (e.m_name == nullptr)
                newException.m_name = m_name;

            throw newException;
        }

        std::string message = e.m_name == nullptr ? format(e) : format(e, e.m_name);

        log(FATAL, message);
        finally();
    }

    void MethodExceptionHandler::errorExceptionHandler(const std::exception& e) const
    {
        finally();
        log(ERROR, format(e));
    }

    std::string MethodExceptionHandler::format(const std::exception& e) const
    {
        return format(e, m_name);
    }

    std::string MethodExceptionHandler::format(const std::exception& e, const std::string& handlerName) const
    {
        if (m_name == nullptr)
            return std::string(e.what());

        return std::string("(") + handlerName + ") " + std::string(e.what());
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
