//
// Created by theohenno on 10/14/25.
//

#pragma once

#include "models/PhysicalDeviceCriteria.h"
#include "models/QueueFamilyIndices.h"

#include "VulkanQueue.h"

namespace narc_engine {
    class IVulkanDeviceConfigProvider;
    class VulkanInstance;
    class IDeviceService;
    class IDeviceQueueService;

    class VulkanDevice final : public narc_core::IInitialisable, public std::enable_shared_from_this<VulkanDevice>
    {
    public:
        explicit VulkanDevice(NARC_DI_IMPORT_SERVICE(IDeviceService),
                              NARC_DI_IMPORT_COMPONENT(VulkanInstance),
                              NARC_DI_IMPORT_SERVICE(IDeviceQueueService));
        ~VulkanDevice() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(const VulkanQueue*, getGraphicsQueue, &m_graphicsQueue);
        NARC_GETTER(const VulkanQueue*, getPresentQueue, &m_presentQueue);

        NARC_GETTER(const QueueFamilyIndices&, getQueueFamilyIndices, m_queueFamilyIndices);

        NARC_GETTER(VkDevice, getHandle, m_device);
        NARC_GETTER(VkPhysicalDevice, getPhysicalDeviceHandle, m_physicalDevice);

        NARC_SETTER(PhysicalDeviceCriteria, PhysicalDeviceCriteria, m_physicalDeviceCriteria);
        
        void waitIdle() const;

    private:
        narc_core::injected_component<VulkanInstance> m_instance;

        narc_core::injected_service<IDeviceService> m_deviceService;
        narc_core::injected_service<IDeviceQueueService> m_queueService;

        PhysicalDeviceCriteria m_physicalDeviceCriteria{};

        // Raw Handles
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;

        VulkanQueue m_graphicsQueue;
        VulkanQueue m_presentQueue;
        
        // Computed Infos
        QueueFamilyIndices m_queueFamilyIndices{};

        // Methods
        void selectPhysicalDeviceFromCriteria();
        void selectQueueFamily();

        void createDevice();
    };
} // namespace narc_engine
