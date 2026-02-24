//
// Created by theo on 2/18/26.
//

#pragma once

namespace narc_engine {
    class ISurface;
    using device_score_t = int;

    NO_DISCARD bool isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria, const ISurface* surface) noexcept;

    NO_DISCARD device_score_t evaluateDeviceScore(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) noexcept;

    bool areAllRequiredExtensionsAvailable(const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                                  const std::vector<VkExtensionProperties>&
                                                  availableExtensions);

    NO_DISCARD bool areDeviceExtensionsSupported(const VkPhysicalDevice& device,
                                                const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                                const ISurface* surface) noexcept;

    NO_DISCARD VulkanServiceQuery<VkPhysicalDevice> queryBestPhysicalDevices(std::vector<VkPhysicalDevice> devices,
                                                                             const PhysicalDeviceCriteria& criteria,
                                                                             const ISurface* surface) noexcept;
}
