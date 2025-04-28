#pragma once

#include <vulkan/vulkan_core.h>

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

        GETTER const inline VkPhysicalDevice &getPhysicalDevice() const { return m_physicalDevice; }
        GETTER const inline QueueFamilyIndices& getQueueFamilyIndices() const { return m_queueFamilyIndices; }
        GETTER const inline SwapChainSupportDetails getSwapChainSupport() const { return querySwapChainSupport(m_physicalDevice); }

    private:
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        const EngineInstance *m_instance = nullptr;
        QueueFamilyIndices m_queueFamilyIndices;

        const ISurfaceProvider* m_surface = nullptr;

    private:
        void getAllPhysicalDevices(uint32_t *pPhysicalDeviceCount, VkPhysicalDevice *pPhysicalDevices) const;
        int rateDeviceSuitability(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice) const;
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
        VkBool32 isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;
        bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char *> &deviceExtensions);
    };
}