//
// Created by theo on 2/17/26.
//

#pragma once

#include "device/VulkanDevice.h"

namespace narc_engine {
    class VulkanGraphicsInstance : public IGraphicsInstance
    {
    public:
        VulkanGraphicsInstance();
        ~VulkanGraphicsInstance() noexcept override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(const IQueue*, getGraphicsQueue, m_device->getGraphicsQueue())
        NARC_OVERRIDE_GETTER(const IQueue*, getPresentQueue, m_device->getPresentQueue())

        void setApplicationInfo(const ApplicationInfo& value) noexcept override;
        void setDeviceCriteria(const PhysicalDeviceCriteria& value) noexcept override;

        NARC_QUERY_OVERRIDE(std::unique_ptr<ISurface>, createSurface, const IWindow* window);
        NARC_QUERY_OVERRIDE(std::unique_ptr<ISwapchain>, createSwapChain, const ISurface* surface);

        void attachWindow(const IWindow* window) noexcept override;

    private:
        const IWindow* m_mainWindow;

        std::unique_ptr<VulkanInstance> m_instance;
        std::unique_ptr<VulkanDevice> m_device;
    };
}
