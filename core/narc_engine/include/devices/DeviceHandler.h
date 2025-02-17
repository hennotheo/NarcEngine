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

        void create(const Window* window, const VkInstance& instance, const EngineDebugLogger& debugLogger);

        void createSwapChain(VkSwapchainCreateInfoKHR& createInfo, VkSwapchainKHR* swapchain) const;
        void createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateInfo poolInfo) const;
        VkShaderModule createShaderModule(const std::vector<char>& code) const;
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        void destroyShaderModule(VkShaderModule shaderModule) const;

        VkResult waitDeviceIdle() const;
        VkResult submitGraphicsQueue(uint32_t submitCount, const VkSubmitInfo* submitInfo, VkFence fence) const;
        VkResult presentKHR(const VkPresentInfoKHR* presentInfo) const;
        void waitGraphicsQueueIdle() const;

        void release();

    private:
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;

        VkPhysicalDeviceProperties m_physicalDeviceProperties{};

        VkInstance m_vulkanInstance;
        const Window* m_window = nullptr;

        void pickPhysicalDevice();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
        void createLogicalDevice(const EngineDebugLogger& debugLogger);
        int rateDeviceSuitability(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions);
    };
}
