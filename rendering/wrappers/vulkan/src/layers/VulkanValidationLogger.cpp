//
// Created by theohenno on 10/22/25.
//

#include "layers/VulkanValidationLogger.h"

namespace narc_engine {
    using VulkanDebugCallbackUserDataPtr = void*;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback( //TODO: TEMP
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            VulkanDebugCallbackUserDataPtr pUserData)
    {
        switch (messageSeverity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                NARC_LOG_DEBUG("Vk: {}", pCallbackData->pMessage);
                break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                NARC_LOG_INFO("Vk: {}", pCallbackData->pMessage);
                break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                NARC_LOG_WARNING("Vk: {}", pCallbackData->pMessage);
                break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                NARC_LOG_ERROR("Vk: {}", pCallbackData->pMessage);
                break;

            default:
                NARC_LOG_ERROR("Vk: {}", pCallbackData->pMessage);
        }

        return VK_FALSE;
    }

    VulkanValidationLogger::VulkanValidationLogger(NARC_DI_IMPORT_COMPONENT(VulkanInstance)) :
        NARC_DI_IMPL_COMPONENT(VulkanInstance, m_instance)
    {
        m_debugUtilsMessengerCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
                .pfnUserCallback = debugCallback,
                .pUserData = nullptr
        };
    }

    VulkanValidationLogger::~VulkanValidationLogger() = default;

    void VulkanValidationLogger::init()
    {
        if (const auto result = createVulkanUtilsMessenger(&m_debugUtilsMessengerCreateInfo, nullptr);
            result != VK_SUCCESS)
        {
            NARC_LOG_FATAL("Can't create Utils Messenger");
        }
    }

    void VulkanValidationLogger::shutdown()
    {
        destroyVulkanUtilsMessenger(nullptr);
    }

    VkResult VulkanValidationLogger::createVulkanUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT* createInfos,
                                                                const VkAllocationCallbacks* allocator)
    {
        const auto instance = m_instance->getHandled();
        if (const auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"); func != nullptr)
        {
            return func(instance, createInfos, allocator, &m_debugUtilsMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void VulkanValidationLogger::destroyVulkanUtilsMessenger(const VkAllocationCallbacks* allocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance->getHandled(), "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_instance->getHandled(), m_debugUtilsMessenger, allocator);
            m_debugUtilsMessenger = VK_NULL_HANDLE;
        }
    }
}
