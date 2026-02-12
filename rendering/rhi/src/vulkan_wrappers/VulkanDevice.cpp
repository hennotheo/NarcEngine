//
// Created by theohenno on 10/14/25.
//

#include "vulkan_wrappers/VulkanDevice.h"

#include "vulkan_wrappers/VulkanInstance.h"

#include "layers/VulkanGlfwExtension.h"
#include "layers/VulkanValidationLogger.h"

namespace narc_engine {
    VulkanDevice::VulkanDevice(NARC_DI_IMPORT_SERVICE(IDeviceService),
                               NARC_DI_IMPORT_COMPONENT(VulkanInstance),
                               NARC_DI_IMPORT_SERVICE(IDeviceQueueService)) :
        NARC_DI_IMPL_COMPONENT(VulkanInstance, m_instance),
        NARC_DI_IMPL_SERVICE(IDeviceService, m_deviceService),
        NARC_DI_IMPL_SERVICE(IDeviceQueueService, m_queueService)
    {
        //Empty constructor.
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
        const auto devices = m_deviceService->queryAllPhysicalDevices();
        if (!devices.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        const auto bestDeviceResult = m_deviceService->queryBestPhysicalDevices(devices.value(), m_physicalDeviceCriteria);
        if (!bestDeviceResult.has_value())
        {
            NARC_ERROR_RUNTIME("No suitable device found!");
        }

        m_physicalDevice = bestDeviceResult.value();
        vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
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
        const auto uniqueQueueFamilies = m_queueService->getUniqueIndices(m_queueFamilyIndices).transform_error(
                [](const auto& err) {
                    NARC_ERROR_RUNTIME("Uniques queues not supported by current device");
                    return err;
                }).value();

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

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

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
