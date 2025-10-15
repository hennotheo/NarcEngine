//
// Created by theohenno on 10/15/25.
//

#include "services/PhysicalDeviceService.h"

namespace narc_engine {
    PhysicalDeviceService::PhysicalDeviceService(std::weak_ptr<VulkanInstance> instance) :
        m_instance(std::move(instance))
    {
    }

    PhysicalDeviceService::~PhysicalDeviceService() = default;

    std::vector<VkPhysicalDevice> PhysicalDeviceService::queryAllPhysicalDevices() const
    {
        NARC_GUARD_WEAK(instance, m_instance, "Failed to create PhysicalDeviceService");

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance->getHandled(), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            NARC_ERROR_RUNTIME("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance->getHandled(), &deviceCount, devices.data());

        return devices;
    }

    std::expected<VkPhysicalDevice, QueryBestPhysicalDeviceError> PhysicalDeviceService::queryBestPhysicalDevices(
            std::vector<VkPhysicalDevice> devices) const noexcept
    {
        return std::unexpected(QueryBestPhysicalDeviceError{"No suitable GPU found."});
    }
} // narc_engine
