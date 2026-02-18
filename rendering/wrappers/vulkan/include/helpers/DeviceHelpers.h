//
// Created by theo on 2/18/26.
//

#pragma once

namespace narc_engine {
    using device_score_t = int;

    NO_DISCARD bool isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) noexcept;

    NO_DISCARD device_score_t evaluateDeviceScore(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) noexcept;

    bool areAllRequiredExtensionsAvailable(const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                                  const std::vector<VkExtensionProperties>&
                                                  availableExtensions);

    NO_DISCARD bool areDeviceExtensionSupported(const VkPhysicalDevice& device,
                                                       const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions) noexcept;

    NO_DISCARD VulkanServiceQuery<VkPhysicalDevice> queryBestPhysicalDevices(std::vector<VkPhysicalDevice> devices,
                                                                                    const PhysicalDeviceCriteria& criteria) noexcept;
}
