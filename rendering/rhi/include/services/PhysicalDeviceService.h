//
// Created by theohenno on 10/15/25.
//

#pragma once

#include "models/PhysicalDeviceCriteria.h"

#include "VulkanInstance.h"

namespace narc_engine {
    using QueryDeviceError = std::string;

    class PhysicalDeviceService
    {
    public:
        explicit PhysicalDeviceService(std::weak_ptr<VulkanInstance> instance);
        ~PhysicalDeviceService();

        QUERY(std::vector<VkPhysicalDevice>, QueryDeviceError) queryAllPhysicalDevices() const;
        QUERY(VkPhysicalDevice, QueryDeviceError) queryBestPhysicalDevices(std::vector<VkPhysicalDevice> devices,
                                                                                    const PhysicalDeviceCriteria& criteria) const noexcept;

    private:
        std::weak_ptr<VulkanInstance> m_instance;
    };
} // narc_engine
