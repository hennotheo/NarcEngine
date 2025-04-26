#pragma once
#include <vulkan/vulkan_core.h>

#include "EngineDebugLogger.h"
#include "QueueFamilyIndices.h"

class ImGui_ImplVulkan_InitInfo;

namespace narc_engine
{
    class Window;
    class ISurfaceProvider;

    class DeviceHandler
    {
    public:
        DeviceHandler(const ISurfaceProvider* surface, const EngineInstance* instance, const EngineDebugLogger* debugLogger);
        ~DeviceHandler();

        GETTER const inline VkDevice& getDevice() const { return m_device; }
        GETTER const inline VkPhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
        GETTER const inline VkPhysicalDeviceProperties& getPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }
        GETTER const inline SwapChainSupportDetails getSwapChainSupportDetails() const { return querySwapChainSupport(m_physicalDevice); }

        void createSwapChain(VkSwapchainCreateInfoKHR& createInfo, VkSwapchainKHR* swapchain) const;
        void createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateInfo poolInfo) const;
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;
        void setupImGui(ImGui_ImplVulkan_InitInfo* initInfo) const;

        void waitDeviceIdle() const;
        void waitGraphicsQueueIdle() const;
        VkResult submitGraphicsQueue(uint32_t submitCount, const VkSubmitInfo* submitInfo, VkFence fence) const;
        VkResult presentKHR(const VkPresentInfoKHR* presentInfo) const;

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        VkFormat findDepthFormat() const;
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

    private:
        VkDevice m_device;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;
        QueueFamilyIndices m_queueFamilyIndices;

        VkPhysicalDeviceProperties m_physicalDeviceProperties{};

        const EngineInstance* m_instance = nullptr;
        const ISurfaceProvider* m_surface = nullptr;

        void pickPhysicalDevice();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
        void createLogicalDevice(const EngineDebugLogger* debugLogger);
        int rateDeviceSuitability(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions);
        VkBool32 isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) const;
    };
}
