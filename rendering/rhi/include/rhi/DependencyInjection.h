//
// Created by theoh on 6/29/2025.
//

#pragma once


namespace narc_engine
{
    class WindowVulkan;
    class WindowRhi;
    class ContextVulkan;
    class ContextRhi;

    using namespace boost;

    inline auto createDependencyInjector(RendererApiType apiType)
    {
        return di::make_injector(
            di::bind<ContextRhi>().to<ContextVulkan>(),
            di::bind<WindowRhi>().to<WindowVulkan>());
    }
} // namespace narc_engine
