#pragma once

#include <NarcEngine.h>

namespace narc_engine
{
    class NARC_ENGINE_API IEngine : public narc_core::IInitialisable
    {
        friend class EngineBinder;

    public:
        ~IEngine() override = default;

        NARC_PURE_VIRTUAL_GETTER(EngineBinder*, binder);
        NARC_PURE_VIRTUAL_BOOL_GETTER(shouldClose);

        virtual void pollEvents() = 0;
        virtual void render() = 0;
        virtual void waitDeviceIdle() = 0;
    };

    NARC_ENGINE_API IEngine* getEngine();
    NARC_ENGINE_API IEngine* createEngine();
    
    #pragma warning "temporary resource functions"
    NARC_ENGINE_API ResourceId createMaterial(const char* texturePath);
    NARC_ENGINE_API ResourceId createMesh(const char* modelPath);
}