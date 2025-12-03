//
// Created by theohenno on 10/11/25.
//

#include "vulkan_wrappers/VulkanInstance.h"

#include "layers/VulkanGlfwExtension.h"
#include "layers/VulkanValidationLogger.h"

namespace narc_engine {
    VulkanInstance::VulkanInstance(NARC_DI_IMPORT_SERVICE(IInstanceService)) :
        NARC_DI_IMPL_SERVICE(IInstanceService, m_instanceService)
    {
    }

    VulkanInstance::~VulkanInstance() = default;

    void VulkanInstance::init()
    {
        m_extensions.emplace_back(std::make_unique<VulkanValidationLogger>(shared_from_this()));
        m_extensions.emplace_back(std::make_unique<VulkanGlfwExtension>());

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
} // namespace narc_engine
