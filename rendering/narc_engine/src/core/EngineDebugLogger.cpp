#include "core/EngineDebugLogger.h"

#include <NarcLog.h>

#include "core/EngineInstance.h"

namespace narc_engine {
    const std::vector<const char*> g_validationLayers =
    {
        "VK_LAYER_KHRONOS_validation"
    };

    EngineDebugLogger::EngineDebugLogger(const EngineInstance* instance)
    {
        m_instance = instance;
#ifdef ENABLE_VALIDATION_LAYERS
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        m_instance->createDebugUtilsMessenger(&createInfo, nullptr, &m_debugMessenger);
#endif
    }

    EngineDebugLogger::~EngineDebugLogger()
    {
#ifdef ENABLE_VALIDATION_LAYERS
        m_instance->destroyDebugUtilsMessenger(m_debugMessenger, nullptr);
#endif
    }

    void EngineDebugLogger::linkToInstance(VkInstanceCreateInfo& createInfo,
                                           VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo)
    {
#ifdef ENABLE_VALIDATION_LAYERS
        createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
        createInfo.ppEnabledLayerNames = g_validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
#else
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
#endif
    }

    std::vector<const char*> EngineDebugLogger::getRequiredExtensions()
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

    void EngineDebugLogger::linkToDevice(VkDeviceCreateInfo& createInfo)
    {
#ifdef ENABLE_VALIDATION_LAYERS
        createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
        createInfo.ppEnabledLayerNames = g_validationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif
    }

    void EngineDebugLogger::checkValidationLayerSupport()
    {
#ifdef ENABLE_VALIDATION_LAYERS

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const auto& layerName: g_validationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties: availableLayers)
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

    void EngineDebugLogger::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL EngineDebugLogger::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            NARCLOG_PREPARE_HANDLER(NarcEngineCore);
            NARC_EXECUTE_HANDLED(NarcEngineCore, NARCLOG_ERROR(pCallbackData->pMessage), pCallbackData);
        }
        else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            NARCLOG_WARNING(pCallbackData->pMessage);
        }
        else
        {
            NARCLOG_DEBUG(pCallbackData->pMessage);
        }

        return VK_FALSE;
    }
}
