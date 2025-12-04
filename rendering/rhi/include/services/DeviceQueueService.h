//
// Created by theohenno on 10/21/25.
//

#pragma once

#include "IVulkanSurface.h"
#include "models/QueueFamilyIndices.h"

namespace narc_engine {
    class VulkanSurfacesManager;

    using QueryQueueError = std::string;

    class DeviceQueueService final : public IDeviceQueueService
    {
    public:
        explicit DeviceQueueService(std::weak_ptr<VulkanSurfacesManager> surfacesManager);
        ~DeviceQueueService() override;

        NARC_QUERY_OVERRIDE(VulkanServiceQuery<QueueFamilyIndices>, queryQueueFamilyIndices, const VkPhysicalDevice& physicalDevice);
        NARC_QUERY_OVERRIDE(bool, queueFamilyIndexSupportPresentation,
                            const IVulkanSurface* surface,
                            const VkPhysicalDevice& physicalDevice,
                            uint32_t queueFamilyIndex);

        NARC_QUERY_OVERRIDE(VulkanServiceQuery<std::vector<QueueFamilyIndex>>, getUniqueIndices, const QueueFamilyIndices& queueFamilyIndices);

        void fillQueues(const std::weak_ptr<VulkanDevice>& device, const QueueFamilyIndices& queueFamilyIndices, VulkanQueue& graphicsQueue,
                        VulkanQueue& presentQueue) const override;

    private:
        std::weak_ptr<VulkanSurfacesManager> m_surfacesManager;

        QUERY(std::vector<VkQueueFamilyProperties>, QueryQueueError) queryQueueFamilyProperties(const VkPhysicalDevice& physicalDevice) const;
    };
} // narc_engine
