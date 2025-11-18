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

            // TODO: Temporary Off-screen
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

    /**
     * Populate two VulkanQueue objects with device and family/index values.
     *
     * Sets a weak device reference on \p graphicsQueue and \p presentQueue, sets their
     * queue indices to 0, and assigns family indices from \p queueFamilyIndices.
     * If an index is missing, \c QUEUE_INDEX_NONE is used.
     *
     * @param device Weak reference to the VulkanDevice.
     * @param queueFamilyIndices Optional graphics and presentation family indices.
     * @param graphicsQueue Output graphics VulkanQueue to populate.
     * @param presentQueue Output presentation VulkanQueue to populate.
     */
    void DeviceQueueService::fillQueues(const std::weak_ptr<VulkanDevice>& device, const QueueFamilyIndices& queueFamilyIndices,
                                        VulkanQueue& graphicsQueue, VulkanQueue& presentQueue) const
    {
        presentQueue.setDevice(device);
        graphicsQueue.setDevice(device);

        presentQueue.setQueueIndex(0);
        graphicsQueue.setQueueIndex(0);

        presentQueue.setQueueFamilyIndex(queueFamilyIndices.PresentationFamily.value_or(QUEUE_INDEX_NONE));
        graphicsQueue.setQueueFamilyIndex(queueFamilyIndices.GraphicsFamily.value_or(QUEUE_INDEX_NONE));
    }
} // namespace narc_engine
