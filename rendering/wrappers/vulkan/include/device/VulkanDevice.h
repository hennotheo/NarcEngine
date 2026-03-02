//
// Created by theohenno on 10/14/25.
//

#pragma once

#include "VulkanQueue.h"
#include "services/VulkanMemoryAllocator.h"

namespace narc_engine {
    class IVulkanSurface;
    class IVulkanDeviceConfigProvider;
    class VulkanInstance;
    class IDeviceService;
    class IDeviceQueueService;

    class VulkanDevice final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanDevice(const VulkanInstance* instance);
        ~VulkanDevice() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(const VulkanQueue*, getGraphicsQueue, &m_graphicsQueue);
        NARC_GETTER(const VulkanQueue*, getPresentQueue, &m_presentQueue);

        NARC_GETTER(const QueueFamilyIndices&, getQueueFamilyIndices, m_queueFamilyIndices);

        NARC_GETTER(VkDevice, getHandle, m_device);
        NARC_GETTER(VkPhysicalDevice, getPhysicalDeviceHandle, m_physicalDevice);

        NARC_GETTER(VkPhysicalDeviceProperties, getPhysicalDeviceProperties, m_properties);

        NARC_SETTER(PhysicalDeviceCriteria&, setPhysicalDeviceCriteria, m_physicalDeviceCriteria);
        NARC_SETTER(IVulkanSurface*, setMainWindowSurface, m_mainWindowSurface);

        NARC_QUERY(RhiQuery<std::unique_ptr<IBuffer>>, createBuffer, BufferAllocationInfo infos);

        narc_core::result waitForFences(std::span<const IFence*> fences) const;
        narc_core::result resetFences(std::span<const IFence*> fences) const;
        void waitIdle() const;

    private:
        const VulkanInstance* m_instance;

        PhysicalDeviceCriteria m_physicalDeviceCriteria{};
        const IVulkanSurface* m_mainWindowSurface = nullptr;

        // Raw Handles
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;

        VulkanQueue m_graphicsQueue;
        VulkanQueue m_presentQueue;
        VulkanMemoryAllocator m_memoryAllocator;

        VkPhysicalDeviceProperties m_properties{};

        // Computed Infos
        QueueFamilyIndices m_queueFamilyIndices{};

        // Methods
        void selectPhysicalDeviceFromCriteria();
        void selectQueueFamily();

        void createDevice();

        void fillQueues(const QueueFamilyIndices& queueFamilyIndices);
    };
} // namespace narc_engine
