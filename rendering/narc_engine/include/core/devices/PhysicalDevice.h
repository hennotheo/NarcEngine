#pragma once

#include <vulkan/vulkan_core.h>

#include "core/EngineBuilder.h"
#include "core/QueueFamilyIndices.h"
#include "renderer/SwapChainSupportDetails.h"
#include "core/devices/PhysicalDevice.h"

namespace narc_engine
{
    class EngineInstance;
    class EngineBuilder;
    class ISurfaceProvider;

    class PhysicalDevice
    {
    public:
        PhysicalDevice(const EngineBuilder* builder);

        GETTER const inline VkPhysicalDevice& getVkPhysicalDevice() const { return m_physicalDevice; }
        GETTER const inline QueueFamilyIndices& getQueueFamilyIndices() const { return m_queueFamilyIndices; }
        GETTER const inline SwapChainSupportDetails getSwapChainSupport() const { return querySwapChainSupport(m_physicalDevice); }
        GETTER const inline VkPhysicalDeviceProperties& getPhysicalDeviceProperties() const { return m_physicalDeviceProperties; }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        VkFormat findDepthFormat() const;
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

    private:
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        QueueFamilyIndices m_queueFamilyIndices;
        VkPhysicalDeviceProperties m_physicalDeviceProperties{};

        const KeywordList* m_deviceExtensions = nullptr;
        const EngineInstance* m_instance = nullptr;
        const ISurfaceProvider* m_surface = nullptr;

    private:
        void getAllPhysicalDevices(uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const;
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) const;
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
        VkBool32 isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;

        int rateDeviceSuitability(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& deviceExtensions);
    };
}