//
// Created by theohenno on 10/14/25.
//

#pragma once

#include "VulkanInstance.h"

namespace narc_engine {
    struct VulkanDeviceCreationInfos
    {
        VulkanInstance& Instance;
    };

    class VulkanDevice final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanDevice(const VulkanDeviceCreationInfos& creationInfos);
        ~VulkanDevice() override;

        NARC_IMPL_INITIALISABLE();

    private:
        VulkanDeviceCreationInfos m_creationInfos;

        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;

        QUERY std::vector<VkPhysicalDevice> queryAllPhysicalDevices() const;
        QUERY bool isDeviceSuitable(VkPhysicalDevice device);
    };

}
