//
// Created by theohenno on 10/14/25.
//

#include "VulkanDevice.h"

namespace narc_engine {

    VulkanDevice::VulkanDevice(const VulkanDeviceCreationInfos& creationInfos) :
        m_creationInfos(creationInfos)
    {

    }

    VulkanDevice::~VulkanDevice() = default;

    void VulkanDevice::init()
    {
        const auto devices = queryAllPhysicalDevices();

        std::vector<VkPhysicalDevice> suitableDevices(devices.size());
        for (const auto device: devices)
        {
            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);

            if (isDeviceSuitable(device))
            {
                suitableDevices.push_back(device);
            }
        }

        if (suitableDevices.empty())
        {
            NARC_ERROR_RUNTIME("Failed to find a suitable GPU!");
        }
    }

    void VulkanDevice::shutdown()
    {
    }

    std::vector<VkPhysicalDevice> VulkanDevice::queryAllPhysicalDevices() const
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_creationInfos.Instance.getHandled(), &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            NARC_ERROR_RUNTIME("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_creationInfos.Instance.getHandled(), &deviceCount, devices.data());

        return devices;
    }

    bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
    {
        return true;
    }
}
