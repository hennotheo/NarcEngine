//
// Created by theohenno on 10/14/25.
//

#pragma once

#include "VulkanInstance.h"

#include "config_provider/IVulkanDeviceConfigProvider.h"

#include "services/PhysicalDeviceService.h"

namespace narc_engine {
    class VulkanDevice final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanDevice(std::weak_ptr<IVulkanDeviceConfigProvider> config, std::weak_ptr<PhysicalDeviceService> deviceService,
                              std::weak_ptr<VulkanInstance> instance);
        ~VulkanDevice() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkQueue, getGraphicsQueue, m_graphicsQueue);
        NARC_GETTER(VkQueue, getPresentQueue, m_presentQueue);

    private:
        std::weak_ptr<IVulkanDeviceConfigProvider> m_config;
        std::weak_ptr<VulkanInstance> m_instance;
        std::weak_ptr<PhysicalDeviceService> m_deviceService;

        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;

        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;
    };

}
