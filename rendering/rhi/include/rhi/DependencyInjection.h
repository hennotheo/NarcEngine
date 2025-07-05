//
// Created by theoh on 6/29/2025.
//

#pragma once

#include <boost/di.hpp>

#include "rhi/ContextRhi.h"
#include "rhi/DeviceRhi.h"
#include "rhi/SwapChainRhi.h"
#include "rhi/WindowRhi.h"

#include "backend_vulkan/ContextVulkan.h"
#include "backend_vulkan/DeviceVulkan.h"
#include "backend_vulkan/SwapChainVulkan.h"
#include "backend_vulkan/WindowVulkan.h"

namespace narc_engine
{
    inline auto autoCreateVulkanInjector()
    {
        return boost::di::make_injector(boost::di::bind<ContextRhi>().to<ContextVulkan>().in(boost::di::singleton),
                                        boost::di::bind<DeviceRhi>().to<DeviceVulkan>().in(boost::di::singleton),
                                        boost::di::bind<WindowRhi>().to<WindowVulkan>().in(boost::di::singleton),
                                        boost::di::bind<SwapChainRhi>().to<SwapChainVulkan>().in(boost::di::unique));
    }

    inline auto createRhiInjector(const RendererApiType apiType)
    {
        switch (apiType)
        {
        case RendererApiType::Vulkan:
            return autoCreateVulkanInjector();

        case RendererApiType::DirectX12:
            NARCLOG_FATAL("DirectX12 is not supported yet");

        case RendererApiType::OpenGL:
            NARCLOG_FATAL("OpenGL is not supported yet");

        default:
            NARCLOG_FATAL("Unsupported API type");
        }
    }

    using RhiInjector = decltype(createRhiInjector(RendererApiType::Vulkan)); // TODO : Update this later
} // namespace narc_engine
