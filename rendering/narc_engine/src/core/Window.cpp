#include "core/Window.h"

#include <NarcLog.h>

#include "core/EngineInstance.h"

namespace narc_engine {
    constexpr uint32_t g_width = 800;
    constexpr uint32_t g_height = 600;

    Window::Window()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(g_width, g_height, "Narc Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback); //call static function because GLFW does know how to call a member function
    }

    Window::~Window()
    {
        m_engineInstance->destroyGLFWSurface(m_surface, nullptr);

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::init(const EngineInstance* engineInstance)
    {
        m_engineInstance = engineInstance;
        m_engineInstance->createGLFWSurface(m_window, &m_surface, nullptr);
    }

    void Window::update()
    {
        glfwPollEvents();
        m_shouldClose = glfwWindowShouldClose(m_window);
    }

    const char** Window::getRequiredInstanceExtensions(uint32_t* glfwExtensionCount)
    {
        return glfwGetRequiredInstanceExtensions(glfwExtensionCount);
    }

    SwapChainSupportDetails Window::querySwapChainSupport(VkPhysicalDevice device) const
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.Capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.Formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.Formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

        if (presentModeCount != 0)
        {
            details.PresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.PresentModes.data());
        }

        return details;
    }

    void Window::getValidFramebufferSize(int* width, int* height) const
    {
        getFramebufferSize(width, height);
        while (*width == 0 || *height == 0)
        {
            getFramebufferSize(width, height);
            glfwWaitEvents();
        }
    }

    void Window::getFramebufferSize(int* width, int* height) const
    {
        glfwGetFramebufferSize(m_window, width, height);
    }

    VkBool32 Window::isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex) const
    {
        VkBool32 supported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, m_surface, &supported);
        return supported;
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_framebufferResized = true;
    }
}
