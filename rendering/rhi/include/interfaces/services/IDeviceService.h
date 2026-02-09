//
// Created by theohenno on 12/3/25.
//

#pragma once

namespace narc_engine {
    class PhysicalDeviceCriteria;

    class IDeviceService : public IService
    {
    public:
        virtual ~IDeviceService() override = default;

        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<std::vector<VkPhysicalDevice>>, queryAllPhysicalDevices);
        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VkPhysicalDevice>, queryBestPhysicalDevices,
                                std::vector<VkPhysicalDevice> devices,
                                const PhysicalDeviceCriteria& criteria);
        NARC_PURE_VIRTUAL_QUERY(VulkanServiceQuery<VkPhysicalDeviceProperties >, queryDeviceProperties,
                        VkPhysicalDevice device);
    };
}
