//
// Created by theohenno on 10/21/25.
//

#pragma once

#include "IVulkanSurface.h"
#include "models/QueueFamilyIndices.h"

#include "vulkan_wrappers/VulkanQueue.h"
#include "interfaces/services/IInstanceService.h"

namespace narc_engine {
    struct ApplicationInfo;
    class VulkanSurfacesManager;

    class InstanceService final : public IInstanceService
    {
    public:
        InstanceService() = default;
        ~InstanceService() override = default;

        NARC_MAP_OVERRIDE(ApplicationInfo, VkApplicationInfo);
        
        NARC_QUERY_OVERRIDE(VulkanCreationQuery<VkInstance>, createVkInstance,
                        const VkApplicationInfo& appInfos,
                        const std::span<std::unique_ptr<IVulkanExtension>>& extensions,
                        const std::span<const char*>& layerNames);

        void destroyVkInstance(const VkInstance& instance) const noexcept override;
    };

    using QueryQueueError = std::string;

    class DeviceQueueService final
    {
    public:
        explicit DeviceQueueService(std::weak_ptr<VulkanSurfacesManager> surfacesManager);
        ~DeviceQueueService();

        QUERY(QueueFamilyIndices, QueryQueueError) queryQueueFamilyIndices(
                const VkPhysicalDevice& physicalDevice) const;
        NO_DISCARD bool queueFamilyIndexSupportPresentation(const IVulkanSurface* surface, const VkPhysicalDevice& physicalDevice,
                                                            uint32_t queueFamilyIndex) const;

        NO_DISCARD std::vector<QueueFamilyIndex> getUniqueIndices(const QueueFamilyIndices& queueFamilyIndices) const;

        void fillQueues(const std::weak_ptr<VulkanDevice>& device, const QueueFamilyIndices& queueFamilyIndices, VulkanQueue& graphicsQueue,
                        VulkanQueue& presentQueue) const;

    private:
        std::weak_ptr<VulkanSurfacesManager> m_surfacesManager;

        QUERY(std::vector<VkQueueFamilyProperties>, QueryQueueError) queryQueueFamilyProperties(const VkPhysicalDevice& physicalDevice) const;
    };
} // narc_engine
