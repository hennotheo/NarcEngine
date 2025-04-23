#pragma once

#include <NarcEngine.h>

namespace narc_engine
{
    class NARC_ENGINE_API IWindow
    {
        friend class EngineBinder;

    public:
        virtual ~IWindow() = default;

        GETTER virtual double getWindowTime() const = 0;
    };
}
