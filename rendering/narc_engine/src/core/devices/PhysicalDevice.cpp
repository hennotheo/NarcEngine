#include "core/devices/PhysicalDevice.h"

#include "core/EngineInstance.h"
#include "core/interfaces/ISurfaceProvider.h"

namespace narc_engine
{
    const std::vector<const char*> g_deviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
        VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
        VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
    };

    PhysicalDevice::PhysicalDevice(const EngineBuilder* builder)
    {
        m_surface = builder->getSurfaceProvider();
        m_instance = builder->getInstance();
        
        uint32_t deviceCount = 0;
        getAllPhysicalDevices(&deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        getAllPhysicalDevices(&deviceCount, devices.data());
        
        if (deviceCount == 0)
        {
            NARCLOG_FATAL("Failed to find GPUs with Vulkan Support!");
        }
        
        std::multimap<int, VkPhysicalDevice> candidates;
        for (const auto &device : devices)
        {
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0)
        {
            m_physicalDevice = candidates.rbegin()->second;
            m_queueFamilyIndices = findQueueFamilies(m_physicalDevice);
        }
        else
        {
            NARCLOG_FATAL("Failed to find a suitable GPU!");
        }
    }

    void PhysicalDevice::getAllPhysicalDevices(uint32_t *pPhysicalDeviceCount, VkPhysicalDevice *pPhysicalDevices) const
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

        bool extensionsSupported = checkDeviceExtensionSupport(device, g_deviceExtensions);
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