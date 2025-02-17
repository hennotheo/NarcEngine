﻿#include "include/DeviceHandler.h"

namespace narc_engine
{
    const std::vector<const char*> g_deviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    void DeviceHandler::create(const Window* window, const VkInstance& instance, const EngineDebugLogger& debugLogger, VkQueue* graphicsQueue, VkQueue* presentQueue)
    {
        m_vulkanInstance = instance;
        m_window = window;

        pickPhysicalDevice();
        createLogicalDevice(debugLogger, graphicsQueue, presentQueue);

        vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
    }

    VkShaderModule DeviceHandler::createShaderModule(const std::vector<char>& code) const
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }

    void DeviceHandler::destroyShaderModule(VkShaderModule shaderModule) const
    {
        vkDestroyShaderModule(m_device, shaderModule, nullptr);
    }

    uint32_t DeviceHandler::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
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

        throw std::runtime_error("failed to find suitable memory type!");
    }

    void DeviceHandler::createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateInfo poolInfo) const
    {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice);
        
        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void DeviceHandler::waitIdle() const
    {
        vkDeviceWaitIdle(m_device);
    }

    void DeviceHandler::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_vulkanInstance, &deviceCount, nullptr);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_vulkanInstance, &deviceCount, devices.data());

        if (deviceCount == 0)
        {
            throw std::runtime_error("Failed to find GPUs with Vulkan Support!");
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
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
    }

    void DeviceHandler::createLogicalDevice(const EngineDebugLogger& debugLogger, VkQueue* graphicsQueue, VkQueue* presentQueue)
    {
        QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.GraphicsFamily.value(), indices.PresentFamily.value()};

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

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(g_deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = g_deviceExtensions.data();

        debugLogger.linkToDevice(createInfo);

        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(m_device, indices.GraphicsFamily.value(), 0, graphicsQueue);
        vkGetDeviceQueue(m_device, indices.PresentFamily.value(), 0, presentQueue);
    }

    int DeviceHandler::rateDeviceSuitability(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        //Performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        //Max possible size of texture affect graphics quality
        score += deviceProperties.limits.maxImageDimension2D;
        score += deviceFeatures.samplerAnisotropy ? 1000 : 0;

        //Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader)
        {
            return 0;
        }

        bool extensionsSupported = checkDeviceExtensionSupport(device, g_deviceExtensions);
        if (!extensionsSupported)
        {
            return 0;
        }

        bool swapChainAdequate = false;
        SwapChainSupportDetails swapChainSupport = m_window->querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
        if (!swapChainAdequate)
        {
            return 0;
        }

        QueueFamilyIndices indices = findQueueFamilies(device);
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.GraphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        if (!indices.isComplete())
        {
            return 0;
        }

        return score;
    }

    bool DeviceHandler::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions)
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

    QueueFamilyIndices DeviceHandler::findQueueFamilies(VkPhysicalDevice device) const
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

            if (m_window->isPhysicalDeviceSupported(device, i))
            {
                indices.PresentFamily = i;
            }

            if (indices.isComplete())
                break;

            i++;
        }

        return indices;
    }
}
