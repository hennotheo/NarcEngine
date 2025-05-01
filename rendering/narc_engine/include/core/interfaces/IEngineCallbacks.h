#pragma once 

namespace narc_engine {
    class IEngineCallbacks
    {
    public:
        virtual void stop() = 0;
    };
} // namespace narc_engine