//
// Created by theohenno on 10/14/25.
//

#pragma once

#include "VulkanQueue.h"

#include "models/QueueFamilyIndices.h"
#include "services/DeviceQueueService.h"
#include "services/PhysicalDeviceService.h"

namespace narc_engine {
    class IVulkanDeviceConfigProvider;
    class VulkanInstance;

    class VulkanDevice final : public narc_core::IInitialisable, public std::enable_shared_from_this<VulkanDevice>
    {
    public:
        explicit VulkanDevice(std::weak_ptr<IVulkanDeviceConfigProvider> config,
                              const std::shared_ptr<PhysicalDeviceService>& deviceService,
                              std::weak_ptr<VulkanInstance> instance,
                              const std::shared_ptr<DeviceQueueService>& queueService);
        ~VulkanDevice() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(const VulkanQueue*, getGraphicsQueue, &m_graphicsQueue);
        NARC_GETTER(const VulkanQueue*, getPresentQueue, &m_presentQueue);

        NARC_GETTER(const QueueFamilyIndices&, getQueueFamilyIndices, m_queueFamilyIndices);

        NARC_GETTER(VkDevice, getHandle, m_device);
        NARC_GETTER(VkPhysicalDevice, getPhysicalDeviceHandle, m_physicalDevice);

    private:
        // Services
        std::weak_ptr<IVulkanDeviceConfigProvider> m_config;
        std::weak_ptr<VulkanInstance> m_instance;

        std::shared_ptr<PhysicalDeviceService> m_deviceService;
        std::shared_ptr<DeviceQueueService> m_queueService;

        VulkanQueue m_graphicsQueue;
        VulkanQueue m_presentQueue;

        // Raw Handles
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;

        // Computed Infos
        QueueFamilyIndices m_queueFamilyIndices{};

        // Methods
        void selectPhysicalDeviceFromCriteria();
        void selectQueueFamily();

        void createDevice();
    };
} // namespace narc_engine
