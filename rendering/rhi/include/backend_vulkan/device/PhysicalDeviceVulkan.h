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
        VkPhysicalDeviceProperties Properties;
    };

    class PhysicalDeviceVulkan final
    {
    public:
        explicit PhysicalDeviceVulkan(const ContextVulkan* context, const WindowVulkan* window);
        ~PhysicalDeviceVulkan();

        QUERY PhysicalDeviceVulkanProperties queryPhysicalDevice() const;

        SETTER void addExtension(const char* extension) { m_deviceExtensions.insert(extension); }

    private:
        std::vector<VkPhysicalDevice> m_physicalDevices;

        std::set<const char*> m_deviceExtensions;

        const ContextVulkan* m_context;
        const WindowVulkan* m_window;

    private:
        QUERY VkPhysicalDevice queryBestPhysicalDevice() const;

        int rateDeviceSuitability(VkPhysicalDevice device) const;
        RhiResult isSurfaceSupportedByPhysicalDevice(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;
        RhiResult deviceSupportAllRequiredExtensions(VkPhysicalDevice physicalDevice) const;
        QueueFamilyIndicesVulkan findQueueFamilies(VkPhysicalDevice physicalDevice) const;
    };
}
