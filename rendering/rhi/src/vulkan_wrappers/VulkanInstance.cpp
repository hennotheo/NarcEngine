//
// Created by theohenno on 10/11/25.
//

#include "../../include/vulkan_wrappers/VulkanInstance.h"

#include "IVulkanSurface.h"
#include "layers/VulkanGlfwExtension.h"
#include "layers/VulkanValidationLogger.h"

namespace narc_engine {
    VulkanInstance::VulkanInstance(std::weak_ptr<IVulkanInstanceConfigProvider> config) :
        m_config(std::move(config))
    {
    }

    VulkanInstance::~VulkanInstance() = default;

    void VulkanInstance::init()
    {
        NARC_GUARD_WEAK(configPtr, m_config, "Failed to create VulkanInstanceInfos");

        m_extensions.emplace_back(std::make_unique<VulkanValidationLogger>(shared_from_this()));
        m_extensions.emplace_back(std::make_unique<VulkanGlfwExtension>());

        std::vector<const char*> deviceExtensionNames{};
        deviceExtensionNames.reserve(m_extensions.size());
        for (const auto& ext: m_extensions)
        {
            for (const auto names = ext->getExtensionNames();
                 const char* name: names)
            {
                deviceExtensionNames.push_back(name);
            }
        }

        const std::vector layerNames = {
                "VK_LAYER_KHRONOS_validation"
        };

        m_appInfo = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = configPtr->getApplicationName().c_str(),
                .applicationVersion = VK_MAKE_VERSION(0, 2, 0),
                .pEngineName = configPtr->getEngineName().c_str(),
                .engineVersion = VK_MAKE_VERSION(0, 2, 0),
                .apiVersion = VK_API_VERSION_1_3
        };

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &m_appInfo;
        createInfo.enabledLayerCount = layerNames.size();
        createInfo.ppEnabledLayerNames = layerNames.data();
        createInfo.enabledExtensionCount = deviceExtensionNames.size();
        createInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
        createInfo.pNext = m_extensions[0]->getCreationInfos();

        if (const VkResult creationResult = vkCreateInstance(&createInfo, nullptr, &m_instance);
            creationResult != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("CreateInstance failed!");
        }

        NARC_LOG_DEBUG("Vulkan Instance created successfully for application: {}", configPtr->getApplicationName());

        //Init Extensions
    }

    void VulkanInstance::shutdown()
    {
        //Shutdown Extensions

        vkDestroyInstance(m_instance, nullptr);

        m_extensions.clear();

        NARC_LOG_DEBUG("Vulkan Instance destroyed successfully");
    }
} // namespace narc_engine
