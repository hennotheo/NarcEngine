//
// Created by theohenno on 10/11/25.
//

#pragma once

namespace narc_engine {
    class VulkanInstance final : public narc_core::IInitialisable, public std::enable_shared_from_this<VulkanInstance>
    {
    public:
        VulkanInstance();
        ~VulkanInstance() override;

        NARC_IMPL_INITIALISABLE();

        void addExtension(std::unique_ptr<IVulkanExtension> extension);

        NARC_SETTER(ApplicationInfo, setApplicationInfo, m_applicationInfo);

        NARC_GETTER(VkInstance, getHandled, m_instance);
        NARC_GETTER(VkApplicationInfo, getAppInfos, m_appInfo);

        NARC_QUERY(VulkanServiceQuery<std::vector<VkPhysicalDevice>>, queryAllPhysicalDevices);

    private:
        using device_score_t = int;

        ApplicationInfo m_applicationInfo{};

        VkApplicationInfo m_appInfo{};
        VkInstance m_instance = VK_NULL_HANDLE;

        VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

        std::vector<std::unique_ptr<IVulkanExtension>> m_extensions;


        void createVkInstance(const VkApplicationInfo& appInfos,
                              const std::span<std::unique_ptr<IVulkanExtension>>& extensions,
                              const std::span<const char*>& layerNames);

        void destroyVkInstance();
    };
} // namespace narc_engine
