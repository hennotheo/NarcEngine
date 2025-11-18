//
// Created by theohenno on 10/22/25.
//

#include "layers/VulkanValidationLogger.h"

#include <utility>

namespace narc_engine {
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback( //TODO: TEMP
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData)
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

    VulkanValidationLogger::VulkanValidationLogger(std::weak_ptr<VulkanInstance> instance) :
        m_instance(std::move(instance))
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
        createVulkanUtilsMessenger(&m_debugUtilsMessengerCreateInfo, nullptr);
    }

    void VulkanValidationLogger::shutdown()
    {
        destroyVulkanUtilsMessenger(nullptr);
    }

    VkResult VulkanValidationLogger::createVulkanUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT* createInfos,
                                                                const VkAllocationCallbacks* allocator)
    {
        NARC_GUARD_WEAK(instancePtr, m_instance, "Failed to create VulkanValidationLogger");

        const auto instance = instancePtr->getHandled();
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, createInfos, allocator, &m_debugUtilsMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void VulkanValidationLogger::destroyVulkanUtilsMessenger(const VkAllocationCallbacks* allocator)
    {
        NARC_GUARD_WEAK(instancePtr, m_instance, "Failed to create VulkanValidationLogger");

        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instancePtr->getHandled(), "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instancePtr->getHandled(), m_debugUtilsMessenger, allocator);
            m_debugUtilsMessenger = VK_NULL_HANDLE;
        }
    }
}
