#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

namespace NarcEngine
{
    class EngineDebugLogger
    {
    public:
        void Init(VkInstance& instance);

        void LinkToInstance(VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);
        std::vector<const char*> GetRequiredExtensions();
        void LinkToDevice(VkDeviceCreateInfo& createInfo);

        void Clean(VkInstance& instance);

        bool CheckValidationLayerSupport();
        bool CheckDeviceExtensionSupport(VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions);
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    private:
        VkDebugUtilsMessengerEXT m_debugMessenger;


        static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                      void* pUserData);
    };

    inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const
                                                 VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const
                                                 VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT*
                                                 pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
