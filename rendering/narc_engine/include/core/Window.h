#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "renderer/SwapChainSupportDetails.h"

namespace narc_engine {
    class EngineInstance;

    class Window
    {
    public:
        Window();
        ~Window();

        void init(const EngineInstance* engineInstance);

        inline bool isFramebufferResized() const { return m_framebufferResized; }
        inline bool shouldClose() const { return m_shouldClose; }
        inline VkSurfaceKHR getSurface() const { return m_surface; }

        static const char** getRequiredInstanceExtensions(uint32_t* glfwExtensionCount);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
        void getValidFramebufferSize(int* width, int* height) const;
        void getFramebufferSize(int* width, int* height) const;
        VkBool32 isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;

        void update();

        //TODO : MANAGE THIS DIFFERENTLY
        inline void setFramebufferResized(bool value) { m_framebufferResized = value; }

    private:
        GLFWwindow* m_window;
        VkSurfaceKHR m_surface;

        bool m_initialized = false;

        bool m_framebufferResized = false;
        bool m_shouldClose = false;

        const EngineInstance* m_engineInstance;

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    };
}
