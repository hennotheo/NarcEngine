#pragma once

#include "platform/Platforms.h"

namespace narc_engine
{
    class NARC_ENGINE_API ContextRhi
    {
    public:
        ContextRhi() = default;
        virtual ~ContextRhi() = default;

        virtual void init() = 0;
        virtual void shutdown() = 0;
    };

    using ContextRhiPtr = std::unique_ptr<ContextRhi>;

    ContextRhiPtr createContextRhi(RendererApiType api);
} // namespace narc_engine
