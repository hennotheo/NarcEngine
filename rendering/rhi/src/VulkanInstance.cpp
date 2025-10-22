//
// Created by theohenno on 10/11/25.
//

#include "VulkanInstance.h"

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

        std::vector<std::unique_ptr<IVulkanExtension>> extensions;
        extensions.emplace_back(std::make_unique<VulkanValidationLogger>(shared_from_this()));

        std::vector<const char*> deviceExtensionNames{};
        deviceExtensionNames.reserve(extensions.size());
        for (const auto& ext: extensions)
        {
            deviceExtensionNames.push_back(ext->getExtensionName());
        }

        const std::vector layerNames = {
                "VK_LAYER_KHRONOS_validation"
        };

        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = configPtr->getApplicationName().c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 2, 0);
        appInfo.pEngineName = configPtr->getEngineName().c_str();
        appInfo.engineVersion = VK_MAKE_VERSION(0, 2, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.pNext = nullptr;

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = layerNames.size();
        createInfo.ppEnabledLayerNames = layerNames.data();
        createInfo.enabledExtensionCount = deviceExtensionNames.size();
        createInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
        createInfo.pNext = extensions[0]->getCreationInfos();

        if (const VkResult creationResult = vkCreateInstance(&createInfo, nullptr, &m_instance);
            creationResult != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("CreateInstance failed!");
        }

        NARC_LOG_DEBUG("Vulkan Instance created successfully for application: {}", configPtr->getApplicationName());
    }

    void VulkanInstance::shutdown()
    {
        vkDestroyInstance(m_instance, nullptr);

        NARC_LOG_DEBUG("Vulkan Instance destroyed successfully");
    }
} // namespace narc_engine
