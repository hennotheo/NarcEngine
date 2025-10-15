//
// Created by theohenno on 10/15/25.
//

#pragma once
#include "VulkanInstance.h"

namespace narc_engine {
    using QueryBestPhysicalDeviceError = std::string;

    class PhysicalDeviceService
    {
    public:
        explicit PhysicalDeviceService(std::weak_ptr<VulkanInstance> instance);
        ~PhysicalDeviceService();

        QUERY std::vector<VkPhysicalDevice> queryAllPhysicalDevices() const;
        QUERY std::expected<VkPhysicalDevice, QueryBestPhysicalDeviceError> queryBestPhysicalDevices(std::vector<VkPhysicalDevice> devices) const noexcept;

    private:
        std::weak_ptr<VulkanInstance> m_instance;
    };
} // narc_engine
