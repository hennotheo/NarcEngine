//
// Created by theo on 2/17/26.
//

#pragma once

namespace narc_engine {
    class ISwapchain;
    class IQueue;

    class IGraphicsInstance : public narc_core::IInitialisable
    {
    public:
        ~IGraphicsInstance() noexcept override;

        NARC_PURE_VIRTUAL_GETTER(const IQueue*, getGraphicsQueue);
        NARC_PURE_VIRTUAL_GETTER(const IQueue*, getPresentQueue);

        NARC_PURE_VIRTUAL_SETTER(ApplicationInfo, setApplicationInfo);
        NARC_PURE_VIRTUAL_SETTER(PhysicalDeviceCriteria, setDeviceCriteria);

        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<ISurface>, createSurface, const IWindow* window);
        NARC_PURE_VIRTUAL_QUERY(std::unique_ptr<ISwapchain>, createSwapChain, const ISurface* surface);

        virtual void attachWindow(const IWindow* window) noexcept = 0;
    };
}
