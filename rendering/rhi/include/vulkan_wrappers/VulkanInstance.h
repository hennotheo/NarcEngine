//
// Created by theohenno on 10/11/25.
//

#pragma once

#include "interfaces/services/IInstanceService.h"
#include "layers/IVulkanExtension.h"
#include "models/ApplicationInfo.h"

namespace narc_engine {
    class VulkanInstance final : public narc_core::IInitialisable, public std::enable_shared_from_this<VulkanInstance>
    {
    public:
        explicit VulkanInstance(NARC_DI_IMPORT_SERVICE(IInstanceService));
        ~VulkanInstance() override;

        NARC_IMPL_INITIALISABLE();

        NARC_SETTER(ApplicationInfo, ApplicationInfo, m_applicationInfo);

        NARC_GETTER(VkInstance, getHandled, m_instance);
        NARC_GETTER(VkApplicationInfo, getAppInfos, m_appInfo);

    private:
        narc_core::injected_service<IInstanceService> m_instanceService;

        ApplicationInfo m_applicationInfo{};

        VkApplicationInfo m_appInfo{};
        VkInstance m_instance = VK_NULL_HANDLE;

        VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

        std::vector<std::unique_ptr<IVulkanExtension>> m_extensions;
    };
} // namespace narc_engine
