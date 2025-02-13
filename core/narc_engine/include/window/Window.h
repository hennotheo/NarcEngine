#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "include/SwapChainSupportDetails.h"

namespace NarcEngine
{
    class Window
    {
    public:
        void Init();
        void InitSurface(VkInstance instance);

        void Update();

        void CleanSurface(VkInstance m_instance);
        void Clean();

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;
        void GetValidFramebufferSize(int* width, int* height) const;
        void GetFramebufferSize(int* width, int* height) const;
        VkBool32 IsPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex);

        //TODO : MANAGE THIS DIFFERENTLY
        inline void SetFramebufferResized(bool value) { m_framebufferResized = value; }

        inline bool IsFramebufferResized() const { return m_framebufferResized; }
        inline bool ShouldClose() const { return m_shouldClose; }
        inline VkSurfaceKHR GetSurface() const { return m_surface; }

    private:
        GLFWwindow* m_window;

        VkSurfaceKHR m_surface;

        bool m_framebufferResized = false;
        bool m_shouldClose = false;

    private:
        static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
    };
}
