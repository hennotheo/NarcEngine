#pragma once

#include "definitions/RhiExtension.h"
#include "definitions/RhiLayer.h"

#include "WindowRhi.h"
#include "DeviceRhi.h"

namespace narc_engine
{
    NARC_DECL_RHI_PLATFORM_TYPES(Context)

    class NARC_ENGINE_API ContextRhi : public narc_core::IInitialisable
    {
    public:
        ContextRhi();
        ~ContextRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(Context);

        NARC_BOOL_GETTER(isExtensionEnabled, m_extensions.contains(extension), const RhiExtension& extension);
        NARC_BOOL_GETTER(isLayerEnabled, m_layers.contains(layer), const RhiLayer& layer);

        NARC_PURE_VIRTUAL_GETTER(RendererApiType, getRendererApiType);
        NARC_GETTER(const WindowRhi*, getWindow, m_window.get());
        NARC_GETTER(const DeviceRhi*, getDevice, m_device.get());

        NARC_DECL_DEPENDENCY_INJECTION(Window, WindowRhiPtr) { m_window = std::move(dependency); }
        NARC_DECL_DEPENDENCY_INJECTION(Device, DeviceRhiPtr) { m_device = std::move(dependency); }

        virtual void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch) = 0;
        virtual void setApplicationName(const char* name) = 0;

        void addExtension(const RhiExtension& extension);
        void addExtensions(const RhiExtension* extension, size_t count);
        void addLayer(const RhiLayer& layer);
        void addLayers(const RhiLayer* layers, size_t count);

    protected:
        virtual RhiResult enableExtension(const RhiExtension& extension) = 0;
        virtual RhiResult enableLayer(const RhiLayer& extension) = 0;

    private:
        std::unordered_set<RhiExtension> m_extensions;
        std::unordered_set<RhiLayer> m_layers;

        WindowRhiPtr m_window;
        DeviceRhiPtr m_device;
    };

    NARC_DECL_RHI_CREATION(ContextRhi);
} // namespace narc_engine