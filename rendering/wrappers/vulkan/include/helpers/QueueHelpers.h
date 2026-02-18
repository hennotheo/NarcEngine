//
// Created by theo on 2/18/26.
//

#pragma once

namespace narc_engine {
    constexpr uint32_t QUEUE_INDEX_NONE = std::numeric_limits<uint32_t>::max();

    VulkanServiceQuery<QueueFamilyIndices> queryQueueFamilyIndices(const VkPhysicalDevice& physicalDevice) noexcept;

    bool queueFamilyIndexSupportPresentation(const IWindow* surface, const VkPhysicalDevice& physicalDevice,
                                             uint32_t queueFamilyIndex) noexcept;

    VulkanServiceQuery<std::vector<QueueFamilyIndex>> getUniqueIndices(const QueueFamilyIndices& queueFamilyIndices) noexcept;

    VulkanServiceQuery<std::vector<VkQueueFamilyProperties>> queryQueueFamilyProperties(const VkPhysicalDevice& physicalDevice) noexcept;
}
