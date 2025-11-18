//
// Created by theohenno on 10/15/25.
//

#pragma once

#include "IVulkanSurface.h"
#include "models/PhysicalDeviceCriteria.h"

#include "VulkanInstance.h"

namespace narc_engine {
    using QueryDeviceError = std::string;

    class PhysicalDeviceService final
    {
    public:
        explicit PhysicalDeviceService(std::weak_ptr<VulkanInstance> instance);
        ~PhysicalDeviceService();

        QUERY(std::vector<VkPhysicalDevice>, QueryDeviceError) queryAllPhysicalDevices() const noexcept;
        QUERY(VkPhysicalDevice, QueryDeviceError) queryBestPhysicalDevices(std::vector<VkPhysicalDevice> devices,
                                                                           const PhysicalDeviceCriteria& criteria) const noexcept;

    private:
        std::weak_ptr<VulkanInstance> m_instance;

        using device_score_t = int;

        NO_DISCARD bool isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept;
        NO_DISCARD device_score_t evaluateDeviceScore(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept;
    };
} // narc_engine
