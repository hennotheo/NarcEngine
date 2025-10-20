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

    QUERY(std::vector<VkPhysicalDevice>, QueryDeviceError) PhysicalDeviceService::queryAllPhysicalDevices() const
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

    QUERY(VkPhysicalDevice, QueryDeviceError) PhysicalDeviceService::queryBestPhysicalDevices(
            std::vector<VkPhysicalDevice> devices, const PhysicalDeviceCriteria& criteria) const noexcept
    {
        if (devices.empty())
        {
            return std::unexpected(QueryDeviceError{"No suitable GPU found."});
        }

        return devices[0];
    }
} // narc_engine
