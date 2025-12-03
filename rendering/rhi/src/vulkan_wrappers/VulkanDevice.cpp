//
// Created by theohenno on 10/14/25.
//

#include "vulkan_wrappers/VulkanDevice.h"

#include "vulkan_wrappers/VulkanInstance.h"
#include "vulkan_wrappers/VulkanQueue.h"

#include "services/PhysicalDeviceService.h"
#include "services/DeviceQueueService.h"

#include "config_provider/IVulkanDeviceConfigProvider.h"

#include "layers/VulkanGlfwExtension.h"
#include "layers/VulkanValidationLogger.h"

namespace narc_engine {
    VulkanDevice::VulkanDevice(std::weak_ptr<IVulkanDeviceConfigProvider> config,
                               const std::shared_ptr<PhysicalDeviceService>& deviceService,
                               std::weak_ptr<VulkanInstance> instance,
                               const std::shared_ptr<DeviceQueueService>& queueService) :
        m_config(std::move(config)),
        m_instance(std::move(instance)),
        m_deviceService(deviceService),
        m_queueService(queueService)
    {

    }

    VulkanDevice::~VulkanDevice() = default;

    void VulkanDevice::init()
    {
        selectPhysicalDeviceFromCriteria();
        selectQueueFamily();

        createDevice();

        m_queueService->fillQueues(shared_from_this(), m_queueFamilyIndices, m_graphicsQueue, m_presentQueue);

        m_graphicsQueue.init();
        m_presentQueue.init();

        NARC_LOG_DEBUG("Vulkan Device created successfully!");
    }

    void VulkanDevice::shutdown()
    {
        m_presentQueue.shutdown();
        m_graphicsQueue.shutdown();

        vkDestroyDevice(m_device, nullptr);

        m_device = VK_NULL_HANDLE;
        m_physicalDevice = VK_NULL_HANDLE;
    }

    void VulkanDevice::waitIdle() const
    {
        vkDeviceWaitIdle(m_device);
    }

    void VulkanDevice::selectPhysicalDeviceFromCriteria()
    {
        NARC_GUARD_WEAK(configPtr, m_config, "Failed to create VulkanDevice");

        const auto devices = m_deviceService->queryAllPhysicalDevices();
        if (!devices.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        const auto bestDeviceResult = m_deviceService->queryBestPhysicalDevices(devices.value(), configPtr->getPhysicalDeviceCriteria());
        if (!bestDeviceResult.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        m_physicalDevice = bestDeviceResult.value();
    }

    void VulkanDevice::selectQueueFamily()
    {
        const auto queueFamilyIndicesResult = m_queueService->queryQueueFamilyIndices(m_physicalDevice);
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
        const auto uniqueQueueFamilies = m_queueService->getUniqueIndices(m_queueFamilyIndices);

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

        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.enabledExtensionCount = deviceExtensions.size();
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.enabledLayerCount = 0;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create logical device!");
        }
    }
} // namespace narc_engine
