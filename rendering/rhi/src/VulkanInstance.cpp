//
// Created by theohenno on 10/11/25.
//

#include "VulkanInstance.h"

namespace narc_engine {
    VulkanInstance::VulkanInstance() {  }

    VulkanInstance::~VulkanInstance() = default;

    void VulkanInstance::init()
    {
        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "NarcEngine Editor";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 2, 0);
        appInfo.pEngineName = "NarcEngine";
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

        vkCreateInstance(&createInfo, nullptr, &m_instance);
        
        NARC_LOG_DEBUG("Instance initialized.");
    }

    void VulkanInstance::shutdown() { vkDestroyInstance(m_instance, nullptr); }
} // namespace narc_engine
