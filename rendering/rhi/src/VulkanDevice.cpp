//
// Created by theohenno on 10/14/25.
//

#include "VulkanDevice.h"

#include "VulkanInstance.h"

namespace narc_engine {
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> GraphicsFamily;
        std::optional<uint32_t> PresentationFamily;

        NARC_GETTER(bool, isComplete, GraphicsFamily.has_value() && PresentationFamily.has_value());
    };

    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& physicalDevice)
    {
        QueueFamilyIndices indices{};

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily: queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.GraphicsFamily = i;
            }

            //TODO: Temporary Off-screen
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.PresentationFamily = i;
            }

            i++;
        }

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
        if (!devices.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        const auto bestDeviceResult = deviceServivce->queryBestPhysicalDevices(devices.value(), configPtr->getPhysicalDeviceCriteria());
        if (!bestDeviceResult.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        m_physicalDevice = bestDeviceResult.value();
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
        NARC_LOG_DEBUG("Selected GPU: {}", deviceProperties.deviceName);

        NARC_GUARD_WEAK(instancePtr, m_instance, "Failed to create VulkanDevice");

        const auto indices = findQueueFamilies(m_physicalDevice);
        if (!indices.isComplete())
        {
            NARC_ERROR_RUNTIME("Failed to find required queue families!");
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.enabledExtensionCount = 0;
        createInfo.enabledLayerCount = 0;

        // create unique list of queue families to request
        std::vector<uint32_t> uniqueQueueFamilies;
        uniqueQueueFamilies.push_back(indices.GraphicsFamily.value());
        if (indices.PresentationFamily.value() != indices.GraphicsFamily.value())
            uniqueQueueFamilies.push_back(indices.PresentationFamily.value());

        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        queueCreateInfos.reserve(uniqueQueueFamilies.size());
        for (uint32_t family: uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = family;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;


        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create logical device!");
        }

        NARC_LOG_DEBUG("Vulkan Device created successfully!");

        vkGetDeviceQueue(m_device, indices.GraphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, indices.PresentationFamily.value(), 0, &m_presentQueue);
    }

    void VulkanDevice::shutdown()
    {
        vkDestroyDevice(m_device, nullptr);

        m_presentQueue = VK_NULL_HANDLE;
        m_graphicsQueue = VK_NULL_HANDLE;

        m_device = VK_NULL_HANDLE;
        m_physicalDevice = VK_NULL_HANDLE;
    }
}
