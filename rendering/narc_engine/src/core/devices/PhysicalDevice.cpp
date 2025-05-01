#include "core/devices/PhysicalDevice.h"

#include "core/EngineInstance.h"
#include "core/interfaces/ISurfaceProvider.h"

namespace narc_engine
{
    PhysicalDevice::PhysicalDevice(const EngineBuilder* builder)
    {
        m_surface = builder->getSurfaceProvider();
        m_instance = builder->getInstance();
        m_deviceExtensions = builder->getDeviceExtensions();

        uint32_t deviceCount = 0;
        getAllPhysicalDevices(&deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        getAllPhysicalDevices(&deviceCount, devices.data());

        if (deviceCount == 0)
        {
            NARCLOG_FATAL("Failed to find GPUs with Vulkan Support!");
        }

        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto& device : devices)
        {
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0)
        {
            m_physicalDevice = candidates.rbegin()->second;
        }
        else
        {
            NARCLOG_FATAL("Failed to find a suitable GPU!");
        }

        m_queueFamilyIndices = findQueueFamilies(m_physicalDevice);
        vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
    }

    uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        NARCLOG_FATAL("failed to find suitable memory type!");
    }

    void PhysicalDevice::getAllPhysicalDevices(uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const
    {
        if (vkEnumeratePhysicalDevices(m_instance->getvkInstance(), pPhysicalDeviceCount, pPhysicalDevices) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to get physical devices!");
        }
    }

    int PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties deviceProperties;

        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelStructFeatures{};
        VkPhysicalDeviceFeatures2 deviceFeatures2{};
        deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        deviceFeatures2.pNext = &accelStructFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures2(device, &deviceFeatures2);

        int score = 0;

        // Performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        // Max possible size of texture affect graphics quality
        score += deviceProperties.limits.maxImageDimension2D;
        score += deviceFeatures2.features.samplerAnisotropy ? 1000 : 0;

        // Application can't function without geometry shaders
        if (!deviceFeatures2.features.geometryShader)
        {
            return 0;
        }

        bool extensionsSupported = checkDeviceExtensionSupport(device, *m_deviceExtensions);
        if (!extensionsSupported)
        {
            return 0;
        }

        bool swapChainAdequate = false;
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
        if (!swapChainAdequate)
        {
            return 0;
        }

        QueueFamilyIndices indices = findQueueFamilies(device);
        if (!indices.isComplete())
        {
            return 0;
        }

        return score;
    }

    SwapChainSupportDetails PhysicalDevice::querySwapChainSupport(VkPhysicalDevice physicalDevice) const
    {
        SwapChainSupportDetails details;
        VkSurfaceKHR surface = m_surface->getVkSurfaceKHR();

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.Capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.Formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.PresentModes.data());
        }

        return details;
    }

    VkFormat PhysicalDevice::findDepthFormat() const
    {
        const std::vector<VkFormat> candidates = {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT };

        return findSupportedFormat(candidates,
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    VkFormat PhysicalDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
    {
        for (const VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        NARCLOG_FATAL("Failed to find supported format!");
    }

    QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device) const
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.GraphicsFamily = i;
            }

            if (isPhysicalDeviceSupported(device, i))
            {
                indices.PresentFamily = i;
            }

            if (indices.isComplete())
                break;

            i++;
        }

        return indices;
    }

    VkBool32 PhysicalDevice::isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const
    {
        VkBool32 supported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, m_surface->getVkSurfaceKHR(), &supported);
        return supported;
    }

    bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
}