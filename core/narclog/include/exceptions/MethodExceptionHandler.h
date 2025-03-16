//
// Created by theoh on 27/02/2025.
//

#pragma once

#define NARC_EXECUTE_HANDLED(handlerName, method, ...) NARCLOG_HANDLED_METHOD_NAME(handlerName).execute([__VA_ARGS__] { method; })

namespace narclog
{
    class FatalException;

    class NARCLOG_API MethodExceptionHandler final
    {
        friend class ExceptionHandlerBuilder;

    public:
        ~MethodExceptionHandler();

        int invoke();
        int execute(const std::function<void()>& method);

    private:
        explicit MethodExceptionHandler(const std::function<void()>& function,
                                        const std::function<void()>& finallyCallback);

        const std::function<void()> m_function;
        const std::function<void()> m_finallyCallback;

        bool m_handleAllNonFatalExceptionAsFatal = false;
        bool m_rethrowFatal = false;

        const char* m_name = nullptr;

        void fatalExceptionHandler(const FatalException& e);
        void errorExceptionHandler(const std::exception& e) const;
        GETTER std::string format(const std::exception& e) const;
        GETTER std::string format(const std::exception& e, const std::string& handlerName) const;

        void finally() const;
    };
} // narclog
