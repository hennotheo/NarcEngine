//
// Created by theohenno on 11/17/25.
//

#include "GlfwVulkanSurface.h"

// #define VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WAYLAND_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #define GLFW_EXPOSE_NATIVE_WIN32
// #include <GLFW/glfw3native.h>

#include "VulkanInstance.h"

namespace narc_engine {
    GlfwVulkanSurface::GlfwVulkanSurface(std::weak_ptr<VulkanInstance> instance) :
        m_instance(std::move(instance))
    {
    }

    void GlfwVulkanSurface::init()
    {
        if (m_window != nullptr)
        {
            NARC_LOG_WARNING("Double initialisation of glfw window.");
            return;
        }
        
        NARC_GUARD_WEAK(instance, m_instance, "Instance not defined."); //TODO: Set err msg
        
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(640, 480, instance->getAppInfos().pApplicationName, NULL, NULL);

        const auto err = glfwCreateWindowSurface(instance->getHandled(), m_window, nullptr, &m_surface);
        if (err != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void GlfwVulkanSurface::shutdown()
    {
        NARC_GUARD_WEAK(instance, m_instance, "Instance not defined."); //TODO: Set err msg

        vkDestroySurfaceKHR(instance->getHandled(), m_surface, nullptr);
    }

    bool GlfwVulkanSurface::shouldClose() const noexcept
    {
        return glfwWindowShouldClose(m_window);
    }

    VkExtent2D GlfwVulkanSurface::getSurfaceExtent() const noexcept
    {
        int width;
        int height;
        glfwGetFramebufferSize(m_window, &width, &height);

        return {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };
    }
} // narc_engine
