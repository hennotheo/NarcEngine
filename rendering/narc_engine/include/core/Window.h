#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "interfaces/ISurfaceProvider.h"
#include "renderer/SwapChainSupportDetails.h"

namespace narc_engine
{
    class EngineInstance;
    class IEngineCallbacks;

    class Window : public ISurfaceProvider
    {
    public:
        explicit Window(const EngineInstance* engineInstance, IEngineCallbacks* engine);
        ~Window();

        GETTER VkSurfaceKHR getVkSurfaceKHR() const override { return m_surface; }

        GETTER GLFWwindow* getWindow() const { return m_window; }

        inline bool shouldClose() const { return m_shouldClose; }
        inline VkSurfaceKHR getSurface() const { return m_surface; }

        static const char** getRequiredInstanceExtensions(uint32_t* glfwExtensionCount);

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
        void getValidFramebufferSize(int* width, int* height) const;
        void getFramebufferSize(int* width, int* height) const;
        VkBool32 isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const;

        void pollEvents();

    private:
        GLFWwindow* m_window;
        VkSurfaceKHR m_surface;

        narc_core::Event<int, int, int, int> m_onKeyboardEvent;
        narc_core::Event<int, int, int> m_onMouseEvent;

        bool m_initialized = false;

        bool m_shouldClose = false;

        // TODO : REMOVE THIS
        double m_mouseXpos = 0.0;
        double m_mouseYpos = 0.0;
        double m_time = 0.0;

        const EngineInstance* m_engineInstance;
        IEngineCallbacks* m_engine;

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        static void onKeyboardInputPerformed(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void onMouseInputPerformed(GLFWwindow* window, int button, int action, int mods);
    };
}