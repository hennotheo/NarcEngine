#pragma once

#include "definitions/RhiExtension.h"
#include "definitions/RhiLayer.h"

namespace narc_engine
{
    class ContextVulkan;

    class NARC_ENGINE_API ContextRhi : public narc_core::IInitialisable
    {
    public:
        ContextRhi();
        ~ContextRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(Context);

        GETTER bool isExtensionEnabled(const RhiExtension& extension) const { return m_extensions.contains(extension); }
        GETTER bool isLayerEnabled(const RhiLayer& layer) const { return m_layers.contains(layer); }

        virtual void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch) = 0;
        virtual void setApplicationName(const char* name) = 0;

        void addExtensions(const RhiExtension* extension, size_t count);
        void addLayers(const RhiLayer* layers, size_t count);

    protected:
        virtual RhiResult enableExtension(const RhiExtension& extension) = 0;
        virtual RhiResult enableLayer(const RhiLayer& extension) = 0;

    private:
        std::unordered_set<RhiExtension> m_extensions;
        std::unordered_set<RhiLayer> m_layers;
    };

    NARC_DECL_RHI_CREATION(ContextRhi);
} // namespace narc_engine