//
// Created by theo on 2/18/26.
//

#include "helpers/QueueHelpers.h"

namespace narc_engine {

    VulkanServiceQuery<QueueFamilyIndices> queryQueueFamilyIndices(const VkPhysicalDevice& physicalDevice) noexcept
    {
        const auto queueFamilies = queryQueueFamilyProperties(physicalDevice);
        if (!queueFamilies.has_value())
        {
            return vulkanServiceUnexpected(queueFamilies.error());
        }

        uint32_t i = 0;
        QueueFamilyIndices indices{};
        for (const auto& queueFamily: queueFamilies.value())
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.GraphicsFamily = i;
            }

            //TODO: Reimplement presentation
            // if (queueFamilyIndexSupportPresentation(surface, physicalDevice, i))
            // {
            //     indices.PresentationFamily = i;
            // }

            if (indices.isComplete())
            {
                break;
            }

            i++;
        }

        return indices;
    }

    bool queueFamilyIndexSupportPresentation(const ISurface* surface, const VkPhysicalDevice& physicalDevice, uint32_t queueFamilyIndex) noexcept
    {
        if (surface == nullptr || physicalDevice == nullptr)
        {
            return false;
        }

        const auto surfacePtr = surface->getHandle();
        if (surfacePtr == nullptr)
        {
            return false;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, static_cast<VkSurfaceKHR>(surfacePtr), &presentSupport);
        //TODO: CHANGE NULLTPR

        return presentSupport;
    }

    VulkanServiceQuery<std::vector<QueueFamilyIndex>> getUniqueIndices(const QueueFamilyIndices& queueFamilyIndices) noexcept
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

    VulkanServiceQuery<std::vector<VkQueueFamilyProperties>> queryQueueFamilyProperties(const VkPhysicalDevice& physicalDevice) noexcept
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        if (queueFamilyCount == 0)
        {
            return vulkanServiceUnexpected("Physical device has no queue families.");
        }

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        return queueFamilies;
    }
}
