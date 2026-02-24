//
// Created by theo on 2/17/26.
//

#pragma once

#include "VulkanInstance.h"
#include "device/VulkanDevice.h"

namespace narc_engine {
    class ISurface;

    class VulkanGraphicsInstance : public IGraphicsInstance
    {
    public:
        VulkanGraphicsInstance();
        ~VulkanGraphicsInstance() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(const IQueue*, getGraphicsQueue, m_device->getGraphicsQueue())

        void setApplicationInfo(const ApplicationInfo& value) noexcept override;
        void setDeviceCriteria(const PhysicalDeviceCriteria& value) noexcept override;

        void attachWindow(const IWindow* window) noexcept override;

    private:
        std::unique_ptr<VulkanInstance> m_instance;
        std::unique_ptr<VulkanDevice> m_device;

        std::unique_ptr<ISurface> m_surface;
    };
}
