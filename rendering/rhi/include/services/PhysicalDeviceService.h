//
// Created by theohenno on 10/15/25.
//

#pragma once

namespace narc_engine {
    class VulkanSurfacesManager;
    class VulkanInstance;

    class DeviceService final : public IDeviceService
    {
    public:
        explicit DeviceService(NARC_DI_IMPORT_COMPONENT(VulkanInstance),
                               NARC_DI_IMPORT_SERVICE(ISwapchainService),
                               NARC_DI_IMPORT_COMPONENT(VulkanSurfacesManager));
        ~DeviceService() override;

        NARC_QUERY_OVERRIDE(VulkanServiceQuery<std::vector<VkPhysicalDevice>>, queryAllPhysicalDevices);
        NARC_QUERY_OVERRIDE(VulkanServiceQuery<VkPhysicalDevice>, queryBestPhysicalDevices,
                            std::vector<VkPhysicalDevice> devices,
                            const PhysicalDeviceCriteria& criteria);

    private:
        using device_score_t = int;

        narc_core::injected_service<VulkanInstance> m_instance;
        narc_core::injected_service<VulkanSurfacesManager> m_surfacesManager;
        narc_core::injected_service<ISwapchainService> m_swapChainService;

        NO_DISCARD bool isDeviceSuitable(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept;
        NO_DISCARD device_score_t evaluateDeviceScore(const VkPhysicalDevice& device, const PhysicalDeviceCriteria& criteria) const noexcept;
        bool areAllRequiredExtensionsAvailable(const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions,
                                               const std::vector<VkExtensionProperties>&
                                               availableExtensions) const;
        NO_DISCARD bool areDeviceExtensionSupported(const VkPhysicalDevice& device,
                                                    const std::vector<std::shared_ptr<IVulkanExtension>>& requiredExtensions) const noexcept;
    };
} // narc_engine
