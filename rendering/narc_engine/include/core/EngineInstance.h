//
// Created by theoh on 3/2/2025.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GLFWwindow;

namespace narc_engine {
    class EngineInstance final
    {
    public:
        EngineInstance();
        ~EngineInstance();

        GETTER VkInstance getvkInstance() const { return m_instance; }

        void getAllPhysicalDevices(uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const;

        void createGLFWSurface(GLFWwindow* window, VkSurfaceKHR* surface, const VkAllocationCallbacks* pAllocator) const;
        void createDebugUtilsMessenger(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) const;
        void destroyGLFWSurface(VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) const;
        void destroyDebugUtilsMessenger(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) const;

    private:
        VkInstance m_instance;
    };
} // narc_engine
