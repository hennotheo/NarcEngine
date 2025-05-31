//
// Created by theoh on 26/05/2025.
//

#pragma once

#include "QueueFamilyIndicesVulkan.h"
#include "SwapChainSupportDetailsVulkan.h"

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
        SwapChainSupportDetailsVulkan SwapChainSupportDetails;
    };

    class PhysicalDeviceVulkan final
    {
    public:
        explicit PhysicalDeviceVulkan(const ContextVulkan* context);
        ~PhysicalDeviceVulkan();

        QUERY PhysicalDeviceVulkanProperties queryPhysicalDevice();

        SETTER void addExtension(const char* extension) { m_deviceExtensions.insert(extension); }

    private:
        std::vector<VkPhysicalDevice> m_physicalDevices;

        std::set<const char*> m_deviceExtensions;

        const ContextVulkan* m_context;

    private:
        QUERY VkPhysicalDevice queryBestPhysicalDevice() const;

        void registerAllPhysicalDevices();
        int rateDeviceSuitability(VkPhysicalDevice device) const;
        RhiResult isSurfaceSupportedByPhysicalDevice(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;
        RhiResult deviceSupportAllRequiredExtensions(VkPhysicalDevice physicalDevice) const;
        QueueFamilyIndicesVulkan findQueueFamilies(VkPhysicalDevice physicalDevice) const;
        SwapChainSupportDetailsVulkan querySwapChainSupport(VkPhysicalDevice device) const;
    };
}
