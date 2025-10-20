//
// Created by theohenno on 10/14/25.
//

#include "VulkanDevice.h"

#include "VulkanInstance.h"

namespace narc_engine {
    struct QueueFamilyIndices
    {
        uint32_t graphicsFamily;
    };

    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device)
    {
        QueueFamilyIndices indices{};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily: queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }

            i++;
        }
        // Logic to find queue family indices to populate struct with
        return indices;
    }


    VulkanDevice::VulkanDevice(std::weak_ptr<IVulkanDeviceConfigProvider> config, std::weak_ptr<PhysicalDeviceService> deviceService,
                               std::weak_ptr<VulkanInstance> instance) :
        m_config(std::move(config)),
        m_instance(std::move(instance)),
        m_deviceService(std::move(deviceService))
    {
    }

    VulkanDevice::~VulkanDevice() = default;

    void VulkanDevice::init()
    {
        NARC_GUARD_WEAK(configPtr, m_config, "Failed to create VulkanDevice");
        NARC_GUARD_WEAK(deviceServivce, m_deviceService, "Failed to get PhysicalDeviceService");

        const auto devices = deviceServivce->queryAllPhysicalDevices();
        const auto bestDeviceResult = deviceServivce->queryBestPhysicalDevices(devices, configPtr->getPhysicalDeviceCriteria());

        if (!bestDeviceResult.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        m_physicalDevice = bestDeviceResult.value();

        // std::vector<VkPhysicalDevice> suitableDevices{};
        // for (const auto device: devices)
        // {
        //     VkPhysicalDeviceProperties deviceProperties;
        //     vkGetPhysicalDeviceProperties(device, &deviceProperties);
        //
        //     if (isDeviceSuitable(device))
        //     {
        //         suitableDevices.push_back(device);
        //     }
        // }
        //
        // if (suitableDevices.empty())
        // {
        //     NARC_ERROR_RUNTIME("Failed to find a suitable GPU!");
        // }
        //
        // m_physicalDevice = suitableDevices[0];
        // auto queueFamily = findQueueFamilies(m_physicalDevice);
        //
        // VkDeviceQueueCreateInfo queueCreateInfo{};
        // queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        // queueCreateInfo.queueFamilyIndex = queueFamily.graphicsFamily;
        // queueCreateInfo.queueCount = 1;
        //
        // float queuePriority = 1.0f;
        // queueCreateInfo.pQueuePriorities = &queuePriority;

        NARC_GUARD_WEAK(instancePtr, m_instance, "Failed to create VulkanDevice");

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.enabledExtensionCount = 0;
        createInfo.enabledLayerCount = 0;

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create logical device!");
        }

        NARC_LOG_DEBUG("Vulkan Device created successfully!");
    }

    void VulkanDevice::shutdown()
    {
        vkDestroyDevice(m_device, nullptr);
    }

    bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
    {
        return true;
    }
}
