#include "include/window/Window.h"

#include <stdexcept>

namespace NarcEngine
{
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    void Window::Init()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(WIDTH, HEIGHT, "Narcoleptic Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCallback); //call static function because GLFW does know how to call a member function
    }

    void Window::InitSurface(VkInstance instance)
    {
        if (glfwCreateWindowSurface(instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    void Window::Update()
    {
        glfwPollEvents();
        m_shouldClose = glfwWindowShouldClose(m_window);
    }

    void Window::CleanSurface(VkInstance m_instance)
    {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }

    void Window::Clean()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    SwapChainSupportDetails Window::QuerySwapChainSupport(VkPhysicalDevice device) const
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

    void Window::GetValidFramebufferSize(int* width, int* height) const
    {
        GetFramebufferSize(width, height);
        while (*width == 0 || *height == 0)
        {
            GetFramebufferSize(width, height);
            glfwWaitEvents();
        }
    }

    void Window::GetFramebufferSize(int* width, int* height) const
    {
        glfwGetFramebufferSize(m_window, width, height);
    }

    VkBool32 Window::IsPhysicalDeviceSupported(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex)
    {
        VkBool32 supported = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, m_surface, &supported);
        return supported;
    }

    void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_framebufferResized = true;
    }
}
