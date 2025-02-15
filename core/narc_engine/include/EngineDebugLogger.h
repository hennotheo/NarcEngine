#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

namespace narc_engine
{
    class EngineDebugLogger
    {
    public:
        void init(VkInstance& instance);

        void linkToInstance(VkInstanceCreateInfo& createInfo, VkDebugUtilsMessengerCreateInfoEXT& debugCreateInfo);
        std::vector<const char*> getRequiredExtensions();
        void linkToDevice(VkDeviceCreateInfo& createInfo);

        void clean(VkInstance& instance);

        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions);
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    private:
        VkDebugUtilsMessengerEXT m_debugMessenger;


        static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                      void* pUserData);
    };

    inline void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

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
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}
