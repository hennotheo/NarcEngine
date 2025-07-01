//
// Created by theoh on 25/05/2025.
//

#include "DeviceRhi.h"

#include "ContextRhi.h"

#include "backend_vulkan/DeviceVulkan.h"

namespace narc_engine
{
    DeviceRhi::DeviceRhi(const ContextRhi& ctx) :
        narc_core::IInitialisable()
    {
    }

    DeviceRhi::~DeviceRhi() = default;

    DeviceRhiPtr createDeviceRhi(const RendererApiType api, const ContextRhi& ctx)
    {
        switch (api)
        {
        case RendererApiType::Vulkan:
            return std::make_unique<DeviceVulkan>(ctx);

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
