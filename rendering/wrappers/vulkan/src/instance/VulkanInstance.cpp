//
// Created by theohenno on 10/11/25.
//

#include "instance/VulkanInstance.h"

#include "layers/VulkanValidationLogger.h"

namespace narc_engine {
    VulkanInstance::VulkanInstance(NARC_DI_IMPORT_SERVICE(IInstanceService)) :
        NARC_DI_IMPL_SERVICE(IInstanceService, m_instanceService)
    {
        //Empty constructor.
    }

    VulkanInstance::~VulkanInstance() = default;

    void VulkanInstance::init()
    {
        std::vector layerNames = {
                "VK_LAYER_KHRONOS_validation"
        };

        m_appInfo = m_instanceService->mapApplicationInfoToVkApplicationInfo(m_applicationInfo);

        const auto creationQuery = m_instanceService->createVkInstance(m_appInfo, m_extensions, layerNames);
        if (!creationQuery.has_value())
        {
            NARC_ERROR_RUNTIME("Vulkan Instance creation failed: {}", creationQuery.error());
        }

        m_instance = creationQuery.value();
        
        NARC_LOG_DEBUG("Vulkan Instance created successfully for application: {}", m_applicationInfo.ApplicationName.c_str());

        //Init Extensions
    }

    void VulkanInstance::shutdown()
    {
        //Shutdown Extensions

        m_instanceService->destroyVkInstance(m_instance);
        m_extensions.clear();

        NARC_LOG_DEBUG("Vulkan Instance destroyed successfully");
    }

    void VulkanInstance::addExtension(std::unique_ptr<IVulkanExtension> extension)
    {
        m_extensions.push_back(std::move(extension));
    }
} // namespace narc_engine
