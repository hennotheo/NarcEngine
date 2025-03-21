//
// Created by theoh on 3/2/2025.
//

#include "core/EngineInstance.h"

#include <NarcLog.h>

#include "core/EngineDebugLogger.h"

namespace narc_engine {
    EngineInstance::EngineInstance()
    {
        EngineDebugLogger::checkValidationLayerSupport();

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Narc Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 1);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = EngineDebugLogger::getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        EngineDebugLogger::linkToInstance(createInfo, debugCreateInfo);

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create instance!");
        }
    }

    EngineInstance::~EngineInstance()
    {
        vkDestroyInstance(m_instance, nullptr);
    }

    void EngineInstance::getAllPhysicalDevices(uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const
    {
        if (vkEnumeratePhysicalDevices(m_instance, pPhysicalDeviceCount, pPhysicalDevices) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to get physical devices!");
        }
    }

    void EngineInstance::createGLFWSurface(GLFWwindow* window, VkSurfaceKHR* surface, const VkAllocationCallbacks* pAllocator) const
    {
        if (glfwCreateWindowSurface(m_instance, window, pAllocator, surface) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create window surface!");
        }
    }

    void EngineInstance::destroyGLFWSurface(VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) const
    {
        vkDestroySurfaceKHR(m_instance, surface, pAllocator);
    }

    void EngineInstance::destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) const
    {
        destroyDebugUtilsMessengerEXT(m_instance, debugMessenger, pAllocator);
    }

    void EngineInstance::createDebugUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const
    {
        if (createDebugUtilsMessengerEXT(m_instance, pCreateInfo, pAllocator, pDebugMessenger) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to set up debug messenger!");
        }
    }
} // narc_engine
