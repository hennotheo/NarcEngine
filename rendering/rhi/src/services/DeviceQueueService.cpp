//
// Created by theohenno on 10/21/25.
//

#include "services/DeviceQueueService.h"

namespace narc_engine {
    DeviceQueueService::DeviceQueueService() = default;

    DeviceQueueService::~DeviceQueueService() = default;

    QUERY(QueueFamilyIndices, QueryQueueError) DeviceQueueService::queryQueueFamilyIndices(const VkPhysicalDevice& physicalDevice) const
    {
        QueueFamilyIndices indices{};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily: queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.GraphicsFamily = i;
            }

            //TODO: Temporary Off-screen
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.PresentationFamily = i;
            }

            i++;
        }

        return indices;
    }

    std::vector<QueueFamilyIndex> DeviceQueueService::getUniqueIndices(const QueueFamilyIndices& queueFamilyIndices) const
    {
        std::vector<uint32_t> uniqueQueueFamilies;
        uniqueQueueFamilies.push_back(queueFamilyIndices.GraphicsFamily.value());
        if (queueFamilyIndices.PresentationFamily.value() != queueFamilyIndices.GraphicsFamily.value())
        {
            uniqueQueueFamilies.push_back(queueFamilyIndices.PresentationFamily.value());
        }

        return uniqueQueueFamilies;
    }
} // narc_engine
