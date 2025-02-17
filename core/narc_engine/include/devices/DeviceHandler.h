#pragma once
#include <vulkan/vulkan_core.h>

#include "QueueFamilyIndices.h"
#include "include/EngineDebugLogger.h"

namespace narc_engine
{
    class Window;

    class DeviceHandler
    {
    public:
        const inline VkDevice& getDevice() const { return m_device; }
        const inline VkPhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
        const inline VkPhysicalDeviceProperties& getPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }

        void create(const Window* window, const VkInstance& instance, const EngineDebugLogger& debugLogger, VkQueue* graphicsQueue, VkQueue* presentQueue);

        VkShaderModule createShaderModule(const std::vector<char>& code) const;
        void destroyShaderModule(VkShaderModule shaderModule) const;
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        void createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateInfo poolInfo) const;
        void waitIdle() const;
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

    private:
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        VkPhysicalDeviceProperties m_physicalDeviceProperties{};

        VkInstance m_vulkanInstance;
        const Window* m_window = nullptr;

        void pickPhysicalDevice();
        void createLogicalDevice(const EngineDebugLogger& debugLogger, VkQueue* graphicsQueue, VkQueue* presentQueue);
        int rateDeviceSuitability(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions);
    };
}
