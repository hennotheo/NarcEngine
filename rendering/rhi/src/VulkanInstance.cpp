//
// Created by theohenno on 10/11/25.
//

#include "VulkanInstance.h"

#include <utility>

namespace narc_engine {
    VulkanInstance::VulkanInstance(std::weak_ptr<IVulkanInstanceConfigProvider> config) :
        m_config(std::move(config))
    {
    }

    VulkanInstance::~VulkanInstance() = default;

    void VulkanInstance::init()
    {
        NARC_GUARD_WEAK(configPtr, m_config, "Failed to create VulkanInstanceInfos");

        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = configPtr->getApplicationName().c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 2, 0);
        appInfo.pEngineName = configPtr->getEngineName().c_str();
        appInfo.engineVersion = VK_MAKE_VERSION(0, 2, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        createInfo.enabledExtensionCount = 0;
        createInfo.ppEnabledExtensionNames = nullptr;

        if (const VkResult creationResult = vkCreateInstance(&createInfo, nullptr, &m_instance);
            creationResult != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("CreateInstance failed!");
        }

        NARC_LOG_DEBUG("Vulkan Instance created successfully for application: {}", configPtr->getApplicationName());
    }

    void VulkanInstance::shutdown() { vkDestroyInstance(m_instance, nullptr); }
} // namespace narc_engine
