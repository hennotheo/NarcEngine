#include "include/window/Window.h"

#include <stdexcept>

namespace narc_engine
{
    const uint32_t g_width = 800;
    const uint32_t g_height = 600;

    void Window::init()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(g_width, g_height, "Narcoleptic Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback); //call static function because GLFW does know how to call a member function
    }

    void Window::initSurface(VkInstance instance)
    {
        if (glfwCreateWindowSurface(instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    void Window::update()
    {
        glfwPollEvents();
        m_shouldClose = glfwWindowShouldClose(m_window);
    }

    void Window::cleanSurface(VkInstance m_instance)
    {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }

    void Window::clean()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
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

    VkBool32 Window::isPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
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
