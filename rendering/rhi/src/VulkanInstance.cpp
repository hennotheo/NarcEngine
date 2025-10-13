//
// Created by theohenno on 10/11/25.
//

#include "VulkanInstance.h"

namespace narc_engine {
    VulkanInstance::VulkanInstance(VulkanInstanceInfos creationInfos) :
        m_creationInfos(std::move(creationInfos))
    {

    }

    VulkanInstance::~VulkanInstance() = default;

    void VulkanInstance::init()
    {
        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = m_creationInfos.ApplicationName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 2, 0);
        appInfo.pEngineName = m_creationInfos.EngineName.c_str();
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
    }

    void VulkanInstance::shutdown() { vkDestroyInstance(m_instance, nullptr); }
} // namespace narc_engine
