//
// Created by theohenno on 10/15/25.
//

#pragma once

namespace narc_engine {
    class IVulkanDeviceConfigProvider
    {
    public:
        virtual ~IVulkanDeviceConfigProvider() = default;

        NARC_PURE_VIRTUAL_GETTER(VkPhysicalDevice, getPreferredPhysicalDevice);
        NARC_PURE_VIRTUAL_GETTER(VkPhysicalDevice, getPreferredPhysicalDevice);
    };
}