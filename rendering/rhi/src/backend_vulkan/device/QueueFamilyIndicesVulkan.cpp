//
// Created by theoh on 27/05/2025.
//
#include "backend_vulkan/device/QueueFamilyIndicesVulkan.h"

namespace narc_engine
{
    QueueFamilyIndicesVulkan findQueueFamilies(VkPhysicalDevice device)
    {
        // QueueFamilyIndicesVulkan indices;
        //
        // uint32_t queueFamilyCount = 0;
        // vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        // std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        // vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        //
        // int i = 0;
        // for (const auto& queueFamily : queueFamilies)
        // {
        //     if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        //     {
        //         indices.GraphicsFamily = i;
        //     }
        //
        //     if (isPhysicalDeviceSupported(device, i))
        //     {
        //         indices.PresentFamily = i;
        //     }
        //
        //     if (indices.isComplete())
        //         break;
        //
        //     i++;
        // }
    }
}