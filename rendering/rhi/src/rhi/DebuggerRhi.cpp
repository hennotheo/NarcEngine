//
// Created by theoh on 5/23/2025.
//

#include "rhi/DebuggerRhi.h"

#include "platform/vulkan/DebuggerVulkan.h"

namespace narc_engine
{
    DebuggerRhi::DebuggerRhi() = default;
    DebuggerRhi::~DebuggerRhi() = default;

    DebuggerRhiPtr createDebuggerRhi(const RendererApiType api)
    {
        switch (api)
        {
        case RendererApiType::Vulkan:
            return std::make_unique<DebuggerVulkan>();

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
