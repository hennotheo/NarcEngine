#include "rhi/ContextRhi.h"

#include "platform/Platforms.h"
#include "platform/vulkan/ContextVulkan.h"

namespace narc_engine
{
    ContextRhiPtr createContextRhi(RendererApiType api)
    {
        switch (api)
        {
        case RendererApiType::Vulkan:
            return std::make_unique<ContextVulkan>();

        case RendererApiType::OpenGL:
            NARC_FATAL_OPENGL_NOT_SUPPORTED();
            break;

        case RendererApiType::DirectX12:
            NARC_FATAL_DIRECTX12_NOT_SUPPORTED();
            break;
        default:
            break;
        }
        return nullptr;
    }
}