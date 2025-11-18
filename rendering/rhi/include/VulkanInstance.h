//
// Created by theohenno on 10/11/25.
//

#pragma once

#include "config_provider/EngineConfigProvider.h"
#include "layers/IVulkanExtension.h"

namespace narc_engine {
    class VulkanInstance final : public narc_core::IInitialisable, public std::enable_shared_from_this<VulkanInstance>
    {
    public:
        explicit VulkanInstance(std::weak_ptr<IVulkanInstanceConfigProvider> config);
        ~VulkanInstance() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkInstance, getHandled, m_instance);
        NARC_GETTER(VkApplicationInfo, getAppInfos, m_appInfo);

    private:
        std::weak_ptr<IVulkanInstanceConfigProvider> m_config;

        VkApplicationInfo m_appInfo{};
        VkInstance m_instance = VK_NULL_HANDLE;
        
        VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

        std::vector<std::unique_ptr<IVulkanExtension>> m_extensions;
    };
} // namespace narc_engine
