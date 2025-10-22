//
// Created by theohenno on 10/14/25.
//

#include "VulkanDevice.h"

#include "VulkanInstance.h"

namespace narc_engine {
    VulkanDevice::VulkanDevice(std::weak_ptr<IVulkanDeviceConfigProvider> config, std::weak_ptr<PhysicalDeviceService> deviceService,
                               std::weak_ptr<VulkanInstance> instance, std::weak_ptr<DeviceQueueService> queueService) :
        m_config(std::move(config)),
        m_instance(std::move(instance)),
        m_deviceService(std::move(deviceService)),
        m_queueService(std::move(queueService))
    {
    }

    VulkanDevice::~VulkanDevice() = default;

    void VulkanDevice::init()
    {
        selectPhysicalDeviceFromCriteria();
        selectQueueFamily();

        createDevice();

        NARC_LOG_DEBUG("Vulkan Device created successfully!");

        vkGetDeviceQueue(m_device, m_queueFamilyIndices.GraphicsFamily.value(), 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.PresentationFamily.value(), 0, &m_presentQueue);
    }

    void VulkanDevice::shutdown()
    {
        vkDestroyDevice(m_device, nullptr);

        m_presentQueue = VK_NULL_HANDLE;
        m_graphicsQueue = VK_NULL_HANDLE;

        m_device = VK_NULL_HANDLE;
        m_physicalDevice = VK_NULL_HANDLE;
    }

    void VulkanDevice::selectPhysicalDeviceFromCriteria()
    {
        NARC_GUARD_WEAK(configPtr, m_config, "Failed to create VulkanDevice");
        NARC_GUARD_WEAK(deviceService, m_deviceService, "Failed to get PhysicalDeviceService");

        const auto devices = deviceService->queryAllPhysicalDevices();
        if (!devices.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        const auto bestDeviceResult = deviceService->queryBestPhysicalDevices(devices.value(), configPtr->getPhysicalDeviceCriteria());
        if (!bestDeviceResult.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        m_physicalDevice = bestDeviceResult.value();
    }

    void VulkanDevice::selectQueueFamily()
    {
        NARC_GUARD_WEAK(queueServicePtr, m_queueService, "Failed to create QueueService");

        const auto queueFamilyIndicesResult = queueServicePtr->queryQueueFamilyIndices(m_physicalDevice);
        if (!queueFamilyIndicesResult.has_value())
        {
            NARC_ERROR_RUNTIME("Failed to find required queue families.");
        }

        if (!queueFamilyIndicesResult->isComplete())
        {
            NARC_LOG_WARNING("Queue family indices is not complete.");
        }

        m_queueFamilyIndices = queueFamilyIndicesResult.value();
    }

    void VulkanDevice::createDevice()
    {
        VkPhysicalDeviceFeatures deviceFeatures{};

        NARC_GUARD_WEAK(queueServicePtr, m_queueService, "Failed to create QueueService");
        const auto uniqueQueueFamilies = queueServicePtr->getUniqueIndices(m_queueFamilyIndices);

        float queuePriority = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        queueCreateInfos.reserve(uniqueQueueFamilies.size());
        for (const uint32_t family: uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = family;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.enabledExtensionCount = 0;
        createInfo.enabledLayerCount = 0;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create logical device!");
        }
    }
}
