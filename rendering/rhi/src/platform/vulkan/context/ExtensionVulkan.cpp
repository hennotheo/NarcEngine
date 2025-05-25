//
// Created by theoh on 5/23/2025.
//

#include "platform/vulkan/context/ExtensionVulkan.h"

#include "platform/vulkan/ContextVulkan.h"
#include "rhi/RhiCore.h"

namespace narc_engine
{
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            // NARCLOG_PREPARE_HANDLER(NarcEngineCore);
            // NARC_EXECUTE_HANDLED(NarcEngineCore, NARCLOG_ERROR(pCallbackData->pMessage), pCallbackData);
            NARCLOG_ERROR(pCallbackData->pMessage);
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

    RhiResult isExtensionSupported(const char* extensionName)
    {
        uint32_t count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        std::vector<VkExtensionProperties> props(count);
        vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data());

        for (const auto& prop : props)
        {
            if (std::strcmp(prop.extensionName, extensionName) == 0)
                return RHI_SUCCESS;
        }

        return RHI_FAILURE;
    }

    ExtensionVulkan::ExtensionVulkan(const ContextVulkan* ctx, const char* name) :
        m_context(ctx), m_name(name)
    {
        if (isExtensionSupported(name) == RHI_FAILURE)
        {
            NARCLOG_FATAL("Extension is not supported!");
        }
    }

    DebugExtensionVulkan::DebugExtensionVulkan(const ContextVulkan* ctx) :
        ExtensionVulkan(ctx, VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
    {
        m_createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        m_createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        m_createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        m_createInfo.pfnUserCallback = debugCallback;
    }

    DebugExtensionVulkan::~DebugExtensionVulkan()
    {

    }

    void DebugExtensionVulkan::init()
    {
        const auto createDebugMessengerFunc = NARC_VK_GET_INSTANCE_PROC_ADDR(m_context->getVkInstance(), vkCreateDebugUtilsMessengerEXT);
        if (createDebugMessengerFunc == nullptr)
        {
            NARCLOG_FATAL("Failed to get vkCreateDebugUtilsMessengerEXT function pointer!");
        }

        createDebugMessengerFunc(m_context->getVkInstance(), &m_createInfo, nullptr, &m_debugMessenger);
    }

    void DebugExtensionVulkan::shutdown()
    {
        const auto destroyDebugMessengerFunc = NARC_VK_GET_INSTANCE_PROC_ADDR(m_context->getVkInstance(), vkDestroyDebugUtilsMessengerEXT);
        if (destroyDebugMessengerFunc == nullptr)
        {
            NARCLOG_FATAL("Failed to get vkDestroyDebugUtilsMessengerEXT function pointer!");
        }

        destroyDebugMessengerFunc(m_context->getVkInstance(), m_debugMessenger, nullptr);
    }

}
