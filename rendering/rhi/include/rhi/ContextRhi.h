#pragma once

#include "platform/Platforms.h"

#include "definitions/Extensions.h"

namespace narc_engine
{
    class NARC_ENGINE_API ContextRhi
    {
    public:
        ContextRhi();
        virtual ~ContextRhi();

        virtual void init() = 0;
        virtual void shutdown() = 0;

        GETTER bool isExtensionEnabled(const RhiExtensions& extension) const { return m_extensions.contains(extension); }

        virtual void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch) = 0;
        virtual void setApplicationName(const char* name) = 0;

        void addExtensions(const RhiExtensions* extension, size_t count);

    protected:
        virtual RhiResult enableExtension(const RhiExtensions& extension) = 0;

    private:
        std::unordered_set<RhiExtensions> m_extensions;
    };

    using ContextRhiPtr = std::unique_ptr<ContextRhi>;

    ContextRhiPtr createContextRhi(RendererApiType api);
} // namespace narc_engine
