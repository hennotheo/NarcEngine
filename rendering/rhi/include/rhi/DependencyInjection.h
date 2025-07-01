//
// Created by theoh on 6/29/2025.
//

#pragma once

#include "rhi/ContextRhi.h"
#include "rhi/DeviceRhi.h"
#include "rhi/SwapChainRhi.h"
#include "rhi/WindowRhi.h"

namespace narc_engine
{
    using namespace boost;

    class ContextVulkan;
    class DeviceVulkan;
    class WindowVulkan;
    class SwapChainVulkan;

    inline auto createDependencyInjector(RendererApiType apiType)
    {
        // static auto contextInstance = std::make_shared<ContextVulkan>();

        return di::make_injector(di::bind<ContextRhi>().to<ContextVulkan>().in(di::singleton),
                                 di::bind<DeviceRhi>().to<DeviceVulkan>().in(di::unique), di::bind<WindowRhi>().to<WindowVulkan>().in(di::unique),
                                 di::bind<SwapChainRhi>().to<SwapChainVulkan>().in(di::unique));
    }
} // namespace narc_engine
