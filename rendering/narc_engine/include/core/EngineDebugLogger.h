#pragma once

#include <vulkan/vulkan.h>

#include "Window.h"

namespace narc_engine {
    class EngineDebugLogger
    {
    public:
        EngineDebugLogger(const EngineInstance* instance);
        ~EngineDebugLogger();

        static void linkToDevice(VkDeviceCreateInfo& createInfo);
        static void linkToInstance(VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);

    private:
        VkDebugUtilsMessengerEXT m_debugMessenger;

        const EngineInstance* m_instance = nullptr;


        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                      void* pUserData);
    };

    inline void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    inline VkResult createDebugUtilsMessengerEXT(VkInstance instance, const
                                                 VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const
                                                 VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT*
                                                 pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
