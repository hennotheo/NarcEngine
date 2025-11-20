//
// Created by theohenno on 10/21/25.
//

#include "services/DeviceQueueService.h"

#include "IVulkanSurface.h"

namespace narc_engine {
    DeviceQueueService::DeviceQueueService(std::weak_ptr<IVulkanSurfacesManager> surfacesManager) :
        m_surfacesManager(std::move(surfacesManager))
    {

    }

    DeviceQueueService::~DeviceQueueService() = default;

    QUERY(QueueFamilyIndices, QueryQueueError) DeviceQueueService::queryQueueFamilyIndices(const VkPhysicalDevice& physicalDevice) const
    {
        NARC_GUARD_WEAK(surfacesManager, m_surfacesManager, "Failed to create DeviceQueueService");

        const auto* surface = surfacesManager->getMainSurface();
        const auto queueFamilies = queryQueueFamilyProperties(physicalDevice);
        if (!queueFamilies.has_value())
        {
            return std::unexpected(queueFamilies.error());
        }

        uint32_t i = 0;
        QueueFamilyIndices indices{};
        for (const auto& queueFamily: queueFamilies.value())
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.GraphicsFamily = i;
            }

            if (queueFamilyIndexSupportPresentation(surface, physicalDevice, i))
            {
                indices.PresentationFamily = i;
            }

            if (indices.isComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }

    bool DeviceQueueService::queueFamilyIndexSupportPresentation(const IVulkanSurface* surface, const VkPhysicalDevice& physicalDevice,
                                                                 const uint32_t queueFamilyIndex) const
    {
        if (surface == nullptr || physicalDevice == nullptr)
        {
            return false;
        }

        const auto surfacePtr = surface->getHandled();
        if (surfacePtr == nullptr)
        {
            return false;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surfacePtr, &presentSupport); //TODO: CHANGE NULLTPR

        return presentSupport;
    }

    std::vector<QueueFamilyIndex> DeviceQueueService::getUniqueIndices(const QueueFamilyIndices& queueFamilyIndices) const
    {
        std::vector<uint32_t> uniqueQueueFamilies;

        if (!queueFamilyIndices.GraphicsFamily.has_value())
        {
            return uniqueQueueFamilies;
        }

        uniqueQueueFamilies.push_back(queueFamilyIndices.GraphicsFamily.value());

        const bool presentSupport = queueFamilyIndices.PresentationFamily.has_value();
        if (const bool isSameFamily = queueFamilyIndices.PresentationFamily.value_or(QUEUE_INDEX_NONE) == queueFamilyIndices.GraphicsFamily.
                                      value_or(QUEUE_INDEX_NONE);
            !presentSupport || isSameFamily)
        {
            return uniqueQueueFamilies;
        }

        uniqueQueueFamilies.push_back(queueFamilyIndices.PresentationFamily.value());

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

        presentQueue.setQueueIndex(0); //Cf vulkan doc
        graphicsQueue.setQueueIndex(0);

        presentQueue.setQueueFamilyIndex(queueFamilyIndices.PresentationFamily.value_or(QUEUE_INDEX_NONE));
        graphicsQueue.setQueueFamilyIndex(queueFamilyIndices.GraphicsFamily.value_or(QUEUE_INDEX_NONE));
    }

    QUERY(std::vector<VkQueueFamilyProperties>, QueryQueueError) DeviceQueueService::queryQueueFamilyProperties(
            const VkPhysicalDevice& physicalDevice) const
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        if (queueFamilyCount == 0)
        {
            return std::unexpected<QueryQueueError>("Physical device has no queue families.");
        }

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        // queueFamilies.reserve(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        return queueFamilies;
    }
} // namespace narc_engine
