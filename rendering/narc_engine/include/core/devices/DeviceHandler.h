#pragma once
#include <vulkan/vulkan_core.h>

#include "core/devices/PhysicalDevice.h"
#include "core/devices/LogicalDevice.h"

class ImGui_ImplVulkan_InitInfo;

namespace narc_engine
{
    class Window;
    class ISurfaceProvider;
    class EngineBuilder;

    class DeviceHandler
    {
    public:
        DeviceHandler(const EngineBuilder* builder);
        ~DeviceHandler();

        GETTER const inline VkDevice &getDevice() const { return m_device; }
        GETTER const inline VkPhysicalDevice &getPhysicalDevice() const { return m_physicalDevice.get()->getPhysicalDevice(); }
        GETTER const inline VkPhysicalDeviceProperties &getPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }
        GETTER const inline SwapChainSupportDetails getSwapChainSupportDetails() const { return m_physicalDevice.get()->getSwapChainSupport(); }

        void createSwapChain(VkSwapchainCreateInfoKHR &createInfo, VkSwapchainKHR *swapchain) const;
        void createCommandPool(VkCommandPool *commandPool, VkCommandPoolCreateInfo poolInfo) const;
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;
        void setupImGui(ImGui_ImplVulkan_InitInfo* initInfo) const;

        void waitDeviceIdle() const;
        void waitGraphicsQueueIdle() const;
        VkResult submitGraphicsQueue(uint32_t submitCount, const VkSubmitInfo *submitInfo, VkFence fence) const;
        VkResult presentKHR(const VkPresentInfoKHR *presentInfo) const;

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        VkFormat findDepthFormat() const;
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

    private:
        std::unique_ptr<PhysicalDevice> m_physicalDevice;
        VkDevice m_device;

        VkQueue m_presentQueue;
        VkQueue m_graphicsQueue;

        VkPhysicalDeviceProperties m_physicalDeviceProperties{};

        const EngineInstance *m_instance = nullptr;
        const ISurfaceProvider *m_surface = nullptr;
        LayersPtr m_deviceExtensions;

        void createLogicalDevice(const EngineDebugLogger* debugLogger);
    };
}
