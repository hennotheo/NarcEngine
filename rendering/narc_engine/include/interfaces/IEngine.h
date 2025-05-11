#pragma once

#include <NarcEngine.h>

namespace narc_engine
{
    class NARC_ENGINE_API IEngine
    {
        friend class EngineBinder;

    public:
        virtual ~IEngine() = default;

        GETTER virtual EngineBinder* binder() const = 0;
        GETTER virtual bool shouldClose() const = 0;
        virtual void pollEvents() = 0;
        virtual void render() = 0;
        virtual void waitDeviceIdle() = 0;
    };

    NARC_ENGINE_API IEngine* getEngine();
    NARC_ENGINE_API IEngine* createEngine();
}