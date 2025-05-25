#include "rhi/ContextRhi.h"

#include "Platforms.h"
#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    ContextRhi::ContextRhi() = default;
    ContextRhi::~ContextRhi() = default;

    void ContextRhi::addExtensions(const RhiExtension* extension, const size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            const RhiExtension ext = extension[i];

            if (m_extensions.contains(ext))
            {
                NARCLOG_DEBUG("Extension already enabled");
                continue;
            }

            if (enableExtension(ext) == RHI_FAILURE)
            {
                NARCLOG_WARNING("Failed to enable extension");
                continue;
            }

            m_extensions.insert(ext);
        }
    }

    void ContextRhi::addLayers(const RhiLayer* layers, const size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            const RhiLayer layer = layers[i];

            if (m_layers.contains(layer))
            {
                NARCLOG_DEBUG("Layer already enabled");
                continue;
            }

            if (enableLayer(layer) == RHI_FAILURE)
            {
                NARCLOG_WARNING("Failed to enable layer");
                continue;
            }

            m_layers.insert(layer);
        }
    }

    ContextRhiPtr createContextRhi(const RendererApiType api)
    {
        switch (api)
        {
        case RendererApiType::Vulkan:
            return std::make_unique<ContextVulkan>();

        case RendererApiType::OpenGL:
            NARC_FATAL_OPENGL_NOT_SUPPORTED();

        case RendererApiType::DirectX12:
            NARC_FATAL_DIRECTX12_NOT_SUPPORTED();

        default:
            break;
        }
        return nullptr;
    }
} // namespace narc_engine
