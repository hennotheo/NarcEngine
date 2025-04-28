//
// Created by theoh on 3/2/2025.
//

#include "core/EngineInstance.h"

#include <NarcLog.h>

#include "core/EngineDebugLogger.h"

namespace narc_engine
{
#define MINIMUM_VK_VERSION VK_API_VERSION_1_1

    EngineInstance::EngineInstance(const EngineBuilder* builder)
    {
        glfwInit();

        checkValidationLayerSupport(builder->getValidationLayers());

        m_appInfo = createAppInfo();

        uint32_t instanceVersion = getInstanceVersion();

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &m_appInfo;

        auto extensions = getRequiredExtensions();
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

    uint32_t EngineInstance::getInstanceVersion()
    {
        uint32_t instanceVersion = 0;
        vkEnumerateInstanceVersion(&instanceVersion);
        if (instanceVersion < MINIMUM_VK_VERSION)
        {
            NARCLOG_FATAL("Vulkan 1.1 is not supported by the instance.");
        }

        return instanceVersion;
    }

    VkApplicationInfo EngineInstance::createAppInfo()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Narc Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 2, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = MINIMUM_VK_VERSION;

        return appInfo;
    }

    std::vector<const char*> EngineInstance::getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = Window::getRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef ENABLE_VALIDATION_LAYERS
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    void EngineInstance::checkValidationLayerSupport(LayersPtr validationLayers)
    {
#ifdef ENABLE_VALIDATION_LAYERS

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const auto& layerName : *validationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
                NARCLOG_WARNING("Validation layers requested, but not available!");
        }

#endif
    }
} // narc_engine
