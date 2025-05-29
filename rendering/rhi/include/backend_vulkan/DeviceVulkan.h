//
// Created by theoh on 25/05/2025.
//

#pragma once

#include "DeviceRhi.h"

#include "backend_vulkan/ContextVulkan.h"
#include "backend_vulkan/WindowVulkan.h"
#include "backend_vulkan/device/PhysicalDeviceVulkan.h"

namespace narc_engine
{
    class DeviceVulkan final : public DeviceRhi
    {
    public:
        explicit DeviceVulkan(const ContextVulkan* ctx);
        ~DeviceVulkan() override;

        NARC_IMPL_INITIALISABLE();
        NARC_IMPL_VK_PLATFORM_GETTERS(Device);

        NARC_GETTER(VkDevice, getVkDevice, m_device);

    private:
        VkDevice m_device = VK_NULL_HANDLE;

        PhysicalDeviceVulkan m_physicalDevice;

        std::vector<VkDeviceQueueCreateInfo> createQueueCreateInfos(const QueueFamilyIndicesVulkan& indices);
    };
}
