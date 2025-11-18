//
// Created by theohenno on 10/21/25.
//

#pragma once

#include "IVulkanSurface.h"
#include "models/QueueFamilyIndices.h"

#include "VulkanQueue.h"

namespace narc_engine {
    using QueryQueueError = std::string;

    class DeviceQueueService final
    {
    public:
        explicit DeviceQueueService(std::weak_ptr<IVulkanSurfacesManager> surfacesManager);
        ~DeviceQueueService();

        QUERY(QueueFamilyIndices, QueryQueueError) queryQueueFamilyIndices(
                const VkPhysicalDevice& physicalDevice) const;
        NO_DISCARD bool queueFamilyIndexSupportPresentation(const IVulkanSurface* surface, const VkPhysicalDevice& physicalDevice,
                                                            uint32_t queueFamilyIndex) const;

        NO_DISCARD std::vector<QueueFamilyIndex> getUniqueIndices(const QueueFamilyIndices& queueFamilyIndices) const;

        void fillQueues(const std::weak_ptr<VulkanDevice>& device, const QueueFamilyIndices& queueFamilyIndices, VulkanQueue& graphicsQueue,
                        VulkanQueue& presentQueue) const;

    private:
        std::weak_ptr<IVulkanSurfacesManager> m_surfacesManager;
    };
} // narc_engine
