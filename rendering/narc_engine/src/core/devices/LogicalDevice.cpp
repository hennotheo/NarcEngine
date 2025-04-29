#include "core/devices/LogicalDevice.h"

#include "core/EngineDebugLogger.h"

#include "core/devices/PhysicalDevice.h"
#include "core/QueueFamilyIndices.h"

namespace narc_engine
{
    LogicalDevice::LogicalDevice(const EngineBuilder* builder, const PhysicalDevice* physicalDevice)
    {
        const EngineDebugLogger* debugLogger = builder->getDebugLogger();
        LayersPtr deviceExtensions = builder->getDeviceExtensions();
        m_physicalDevice = physicalDevice;

        QueueFamilyIndices indices = m_physicalDevice->getQueueFamilyIndices();

        NARCLOG_DEBUG("Creating logical device...");
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

        NARCLOG_DEBUG("Creating queue families...");
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }
        NARCLOG_DEBUG("Creating device features...");

        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelStructFeatures{};
        accelStructFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        accelStructFeatures.accelerationStructure = VK_TRUE;

        VkPhysicalDeviceFeatures2 deviceFeatures2{};
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.features.samplerAnisotropy = VK_TRUE;
        deviceFeatures2.pNext = &accelStructFeatures;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pNext = &deviceFeatures2;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions->size());
        createInfo.ppEnabledExtensionNames = deviceExtensions->data();

        NARCLOG_DEBUG("Creating device layers...");
        debugLogger->linkToDevice(createInfo);

        if (vkCreateDevice(m_physicalDevice->getPhysicalDevice(), &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create logical device!");
        }
    }

    LogicalDevice::~LogicalDevice()
    {
        vkDestroyDevice(m_device, nullptr);
    }
}
