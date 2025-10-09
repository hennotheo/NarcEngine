#include "ContextRhi.h"

#include <GLFW/glfw3.h>

#include "Platforms.h"
#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    extern auto createVulkanDependencyInjection();

    ContextRhi::ContextRhi()
    {
        glfwInit();
    }

    ContextRhi::~ContextRhi()
    {
        glfwTerminate();
    }

    void ContextRhi::addExtension(const RhiExtension& extension)
    {
        if (m_extensions.contains(extension))
        {
            NARC_LOG_DEBUG("Extension already enabled");
            return;
        }

        if (enableExtension(extension) == RHI_FAILURE)
        {
            NARC_LOG_WARNING("Failed to enable extension");
            return;
        }

        m_extensions.insert(extension);
    }

    void ContextRhi::addLayer(const RhiLayer& layer)
    {
        if (m_layers.contains(layer))
        {
            NARC_LOG_DEBUG("Layer already enabled");
            return;
        }

        if (enableLayer(layer) == RHI_FAILURE)
        {
            NARC_LOG_WARNING("Failed to enable layer");
            return;
        }

        m_layers.insert(layer);
    }

    void ContextRhi::addExtensions(const RhiExtension* extension, const size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            addExtension(extension[i]);
        }
    }

    void ContextRhi::addLayers(const RhiLayer* layers, const size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            addLayer(layers[i]);
        }
    }

    ContextRhiPtr createContextRhi(const RendererApiType api)
    {
        switch (api)
        {
            using enum RendererApiType;

        case Vulkan:
            return std::make_unique<ContextVulkan>();

        case OpenGL:
            NARC_ERROR_NOT_IMPLEMENTED("OpenGl is not supported yet");

        case DirectX12:
            NARC_ERROR_NOT_IMPLEMENTED("DirectX12 is not supported yet");

        default:
            break;
        }
    }
} // namespace narc_engine
