//
// Created by theoh on 25/05/2025.
//

#include "backend_vulkan/DeviceVulkan.h"

namespace narc_engine
{
    DeviceVulkan::DeviceVulkan(const ContextVulkan* ctx, const WindowVulkan* window) :
        DeviceRhi(ctx, window), m_physicalDevice(ctx, window)
    {

    }

    DeviceVulkan::~DeviceVulkan()
    {

    }

    void DeviceVulkan::init()
    {
        const PhysicalDeviceVulkanProperties specs = m_physicalDevice.queryPhysicalDevice();

        NARCLOG_DEBUG("Initializing Vulkan device : {}", specs.Properties.deviceName);

        const std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = createQueueCreateInfos(specs.QueueFamilyIndices);

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        if (vkCreateDevice(specs.PhysicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create logical device!");
        }
    }

    void DeviceVulkan::shutdown()
    {
        vkDestroyDevice(m_device, nullptr);
    }

    std::vector<VkDeviceQueueCreateInfo> DeviceVulkan::createQueueCreateInfos(const QueueFamilyIndicesVulkan& indices)
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        const std::set<uint32_t> uniqueQueueFamilies = {indices.GraphicsFamily.value(), indices.PresentFamily.value()};

        float queuePriority = 1.0f;
        for (const uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        return queueCreateInfos;
    }
}
