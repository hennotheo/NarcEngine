//
// Created by theoh on 27/05/2025.
//

#include "backend_vulkan/device/PhysicalDeviceVulkan.h"

#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    PhysicalDeviceVulkan::PhysicalDeviceVulkan(const ContextVulkan* context) :
        m_context(context)
    {
        uint32_t deviceCount = 0;
        if (vkEnumeratePhysicalDevices(context->getVkInstance(), &deviceCount, nullptr) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to enumerate physical devices!");
        }

        if (deviceCount == 0)
        {
            NARCLOG_FATAL("Failed to find GPUs with Vulkan Support!");
        }

        m_physicalDevices.resize(deviceCount);
        vkEnumeratePhysicalDevices(context->getVkInstance(), &deviceCount, m_physicalDevices.data());
    }

    PhysicalDeviceVulkan::~PhysicalDeviceVulkan()
    {

    }

    PhysicalDeviceVulkanProperties PhysicalDeviceVulkan::queryPhysicalDevice() const
    {
        PhysicalDeviceVulkanProperties props{};

        props.PhysicalDevice = queryBestPhysicalDevice();
        props.QueueFamilyIndices = findQueueFamilies(props.PhysicalDevice);

        return props;
    }

    VkPhysicalDevice PhysicalDeviceVulkan::queryBestPhysicalDevice() const
    {
        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto& device : m_physicalDevices)
        {
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0)
        {
            return candidates.rbegin()->second;
        }

        NARCLOG_FATAL("Failed to find a suitable GPU!");
    }

    int PhysicalDeviceVulkan::rateDeviceSuitability(VkPhysicalDevice device) const
    {
        return 1;
    }

    RhiResult PhysicalDeviceVulkan::isPhysicalDeviceSupported(const VkPhysicalDevice physicalDevice, const uint32_t queueFamilyIndex) const
    {
        bool supported = false;
        // vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, m_surface->get(), &supported);

        if (!supported)
        {
            return RHI_FAILURE;
        }

        return RHI_SUCCESS;
    }
}
