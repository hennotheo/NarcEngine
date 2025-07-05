#pragma once

#include <vulkan/vulkan_core.h>

#include "core/EngineBuilder.h"
#include "core/queues/QueueFamilyIndices.h"
#include "renderer/SwapChainSupportDetails.h"
#include "core/devices/PhysicalDevice.h"

namespace narc_engine
{
    class EngineInstance;
    class EngineBuilder;
    class ISurfaceProvider;

    class PhysicalDevice : public narc_core::IGetter<VkPhysicalDevice>
    {
    public:
        PhysicalDevice(const EngineBuilder* builder);

        NARC_IMPL_IGETTER(VkPhysicalDevice, m_physicalDevice);

        NARC_GETTER(const QueueFamilyIndices&, getQueueFamilyIndices, m_queueFamilyIndices)
        NARC_GETTER(const VkPhysicalDeviceProperties&, getPhysicalDeviceProperties, m_physicalDeviceProperties)

        QUERY SwapChainSupportDetails querySwapChainSupport() const { return querySwapChainSupport(m_physicalDevice); }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        VkFormat findDepthFormat() const;
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

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
