//
// Created by theoh on 6/29/2025.
//

#pragma once

#include "rhi/ContextRhi.h"
#include "rhi/DeviceRhi.h"
#include "rhi/SwapChainRhi.h"
#include "rhi/WindowRhi.h"

//TODO : Hide the includes below in the rhi/Rhi.h header
//TODO : Create a wrapper for the dependency injector
#include "backend_vulkan/ContextVulkan.h"
#include "backend_vulkan/DeviceVulkan.h"
#include "backend_vulkan/SwapChainVulkan.h"
#include "backend_vulkan/WindowVulkan.h"

namespace narc_engine
{
    using namespace boost;


    inline auto createDependencyInjector(RendererApiType apiType)
    {
        return di::make_injector(di::bind<ContextRhi>().to<ContextVulkan>().in(di::singleton),
                                 di::bind<DeviceRhi>().to<DeviceVulkan>().in(di::unique),
                                 di::bind<WindowRhi>().to<WindowVulkan>().in(di::unique),
                                 di::bind<SwapChainRhi>().to<SwapChainVulkan>().in(di::unique));
    }

    using RhiInjector = decltype(createDependencyInjector(RendererApiType::Vulkan));
} // namespace narc_engine
