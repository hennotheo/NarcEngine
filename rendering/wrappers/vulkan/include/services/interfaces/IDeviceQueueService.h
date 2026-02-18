//
// Created by theohenno on 12/4/25.
//

#pragma once

namespace narc_engine {
    struct QueueFamilyIndices;
    class IWindow;
    class VulkanDevice;
    class VulkanQueue;

    class IDeviceQueueService : public narc_core::IService
    {
    public:
        ~IDeviceQueueService() override = default;

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<QueueFamilyIndices>, queryQueueFamilyIndices, const VkPhysicalDevice& physicalDevice);
        NARC_PURE_VIRTUAL_QUERY(bool, queueFamilyIndexSupportPresentation,
                                const IWindow* surface,
                                const VkPhysicalDevice& physicalDevice,
                                uint32_t queueFamilyIndex);
        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<std::vector<QueueFamilyIndex>>, getUniqueIndices, const QueueFamilyIndices& queueFamilyIndices);
        virtual void fillQueues(const std::weak_ptr<VulkanDevice>& device,
                                const QueueFamilyIndices& queueFamilyIndices,
                                VulkanQueue& graphicsQueue,
                                VulkanQueue& presentQueue) const = 0;
    };
}
