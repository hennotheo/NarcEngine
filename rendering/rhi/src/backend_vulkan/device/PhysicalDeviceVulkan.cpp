//
// Created by theoh on 27/05/2025.
//

#include "backend_vulkan/device/PhysicalDeviceVulkan.h"

#include "backend_vulkan/ContextVulkan.h"
#include "backend_vulkan/WindowVulkan.h"

namespace narc_engine
{
    PhysicalDeviceVulkan::PhysicalDeviceVulkan(const ContextVulkan* context, const WindowVulkan* window) :
        m_context(context), m_window(window)
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
        vkGetPhysicalDeviceProperties(props.PhysicalDevice, &props.Properties);

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

    int PhysicalDeviceVulkan::rateDeviceSuitability(const VkPhysicalDevice device) const
    {
        VkPhysicalDeviceProperties deviceProperties{};
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures2 deviceFeatures{};
        deviceFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        // deviceFeatures.pNext = &accelStructFeatures;
        vkGetPhysicalDeviceFeatures2(device, &deviceFeatures);

        int score = 0;

        // Performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        // Max possible size of texture affect graphics quality
        score += deviceProperties.limits.maxImageDimension2D;
        score += deviceFeatures.features.samplerAnisotropy ? 1000 : 0;

        // Application can't function without geometry shaders
        if (!deviceFeatures.features.geometryShader)
        {
            return 0;
        }

        if (!deviceSupportAllRequiredExtensions(device))
        {
            return 0;
        }

#pragma warning "Swap chain support check is currently disabled, uncomment if needed"
        // bool swapChainAdequate = false;
        // SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        // swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
        // if (!swapChainAdequate)
        // {
        //     return 0;
        // }

        const QueueFamilyIndicesVulkan indices = findQueueFamilies(device);
        if (!indices.isComplete())
        {
            return 0;
        }

        return score;
    }

    QueueFamilyIndicesVulkan PhysicalDeviceVulkan::findQueueFamilies(const VkPhysicalDevice physicalDevice) const
    {
        QueueFamilyIndicesVulkan indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.GraphicsFamily = i;
            }

            if (isSurfaceSupportedByPhysicalDevice(physicalDevice, i))
            {
                indices.PresentFamily = i;
            }

            if (indices.isComplete())
                break;

            i++;
        }

        return indices;
    }

    RhiResult PhysicalDeviceVulkan::isSurfaceSupportedByPhysicalDevice(const VkPhysicalDevice physicalDevice, const uint32_t queueFamilyIndex) const
    {
        VkBool32 supported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, m_window->getVkSurface(), &supported);

        if (!supported)
        {
            return RHI_FAILURE;
        }

        return RHI_SUCCESS;
    }

    bool PhysicalDeviceVulkan::deviceSupportAllRequiredExtensions(const VkPhysicalDevice physicalDevice) const
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<const char*> requiredExtensions(m_deviceExtensions);
        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
}
