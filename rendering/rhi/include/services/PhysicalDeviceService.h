//
// Created by theohenno on 10/15/25.
//

#pragma once

#include "SwapChainService.h"
#include "models/PhysicalDeviceCriteria.h"

namespace narc_engine {
    class VulkanSurfacesManager;
    class VulkanInstance;
    class SwapChainService;

    using QueryDeviceError = std::string;

    class PhysicalDeviceService final
    {
    public:
        explicit PhysicalDeviceService(std::weak_ptr<VulkanInstance> instance,
                                       const std::shared_ptr<SwapChainService>& swapChainService,
                                       std::weak_ptr<VulkanSurfacesManager> surfacesManager);
        ~PhysicalDeviceService();

        QUERY(std::vector<VkPhysicalDevice>, QueryDeviceError) queryAllPhysicalDevices() const noexcept;
        QUERY(VkPhysicalDevice, QueryDeviceError) queryBestPhysicalDevices(std::vector<VkPhysicalDevice> devices,
                                                                           const PhysicalDeviceCriteria& criteria) const noexcept;

    private:
        using device_score_t = int;

        std::weak_ptr<VulkanInstance> m_instance;
        std::weak_ptr<VulkanSurfacesManager> m_surfacesManager;
        std::shared_ptr<SwapChainService> m_swapChainService;

        NO_DISCARD bool isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept;
        NO_DISCARD device_score_t evaluateDeviceScore(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept;
        bool areAllRequiredExtensionsAvailable(const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                               const std::vector<VkExtensionProperties>&
                                               availableExtensions) const;
        NO_DISCARD bool areDeviceExtensionSupported(const VkPhysicalDevice& device,
                                                    const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions) const noexcept;

    };
} // narc_engine
