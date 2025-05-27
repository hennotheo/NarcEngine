//
// Created by theoh on 26/05/2025.
//

#pragma once

#include "QueueFamilyIndicesVulkan.h"

namespace narc_engine
{
    class ContextVulkan;
    class WindowVulkan;

    struct PhysicalDeviceVulkanProperties
    {
        const char* Name;

        VkPhysicalDevice PhysicalDevice;
        QueueFamilyIndicesVulkan QueueFamilyIndices;
    };

    class PhysicalDeviceVulkan final
    {
    public:
        explicit PhysicalDeviceVulkan(const ContextVulkan* context, const WindowVulkan* window);
        ~PhysicalDeviceVulkan();

        QUERY PhysicalDeviceVulkanProperties queryPhysicalDevice() const;

    private:
        std::vector<VkPhysicalDevice> m_physicalDevices;

        const ContextVulkan* m_context;
        const WindowVulkan* m_window;

    private:
        QUERY VkPhysicalDevice queryBestPhysicalDevice() const;

        int rateDeviceSuitability(VkPhysicalDevice device) const;
        RhiResult isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;
        QueueFamilyIndicesVulkan findQueueFamilies(VkPhysicalDevice physicalDevice) const;
    };
}
