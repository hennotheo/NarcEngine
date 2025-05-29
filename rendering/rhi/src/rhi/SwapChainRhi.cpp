//
// Created by theoh on 28/05/2025.
//
#include "SwapchainRhi.h"

#include "WindowRhi.h"
#include "DeviceRhi.h"

#include "backend_vulkan/SwapChainVulkan.h"

namespace narc_engine
{
    SwapChainRhi::SwapChainRhi(const WindowRhi* window, const DeviceRhi* device)
        : m_window(window), m_device(device)
    {

    }

    SwapChainRhi::~SwapChainRhi()
    {
    }

    SwapChainRhiPtr createSwapChainRhi(const RendererApiType api, const WindowRhi* window, const DeviceRhi* device)
    {
        switch (api)
        {
        case RendererApiType::Vulkan:
            return std::make_unique<SwapChainVulkan>(window, device);

        case RendererApiType::OpenGL:
            NARC_FATAL_OPENGL_NOT_SUPPORTED();

        case RendererApiType::DirectX12:
            NARC_FATAL_DIRECTX12_NOT_SUPPORTED();

        default:
            break;
        }
        return nullptr;
    }
}
