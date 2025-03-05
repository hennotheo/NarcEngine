#pragma once
#include <vulkan/vulkan_core.h>

#include "QueueFamilyIndices.h"
#include "EngineDebugLogger.h"

namespace narc_engine {
    class Window;

    class DeviceHandler
    {
    public:
        DeviceHandler(const Window* window, const EngineInstance* instance, const EngineDebugLogger* debugLogger);
        ~DeviceHandler();

        const inline VkDevice& getDevice() const { return m_device; }
        const inline VkPhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
        const inline VkPhysicalDeviceProperties& getPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }

        void createSwapChain(VkSwapchainCreateInfoKHR& createInfo, VkSwapchainKHR* swapchain) const;
        void createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateInfo poolInfo) const;
        VkShaderModule createShaderModule(const std::vector<char>& code) const;
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;

        void waitDeviceIdle() const;
        void waitGraphicsQueueIdle() const;
        VkResult submitGraphicsQueue(uint32_t submitCount, const VkSubmitInfo* submitInfo, VkFence fence) const;
        VkResult presentKHR(const VkPresentInfoKHR* presentInfo) const;

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        VkFormat findDepthFormat() const;
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

        void destroyShaderModule(VkShaderModule shaderModule) const;

    private:
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;

        VkPhysicalDeviceProperties m_physicalDeviceProperties{};

        const EngineInstance* m_instance = nullptr;
        const Window* m_window = nullptr;

        void pickPhysicalDevice();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
        void createLogicalDevice(const EngineDebugLogger* debugLogger);
        int rateDeviceSuitability(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions);
    };
}
