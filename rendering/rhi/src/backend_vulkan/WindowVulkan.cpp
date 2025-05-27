//
// Created by theoh on 27/05/2025.
//
#include "backend_vulkan/WindowVulkan.h"

#include <GLFW/glfw3.h>

#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    std::vector<const char*> getVulkanGLFWRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        return std::vector<const char*>(extensions, extensions + glfwExtensionCount);
    }

    WindowVulkan::WindowVulkan(const ContextVulkan* ctx):
        WindowRhi(ctx)
    {
        if (!glfwVulkanSupported())
        {
            NARCLOG_FATAL("Vulkan is not supported by GLFW!");
        }
    }

    WindowVulkan::~WindowVulkan()
    {

    }

    void WindowVulkan::init()
    {
        createWindow();

        if (glfwCreateWindowSurface(m_context->getContextVulkan()->getVkInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create window surface!");
        }
    }

    void WindowVulkan::shutdown()
    {
        vkDestroySurfaceKHR(m_context->getContextVulkan()->getVkInstance(), m_surface, nullptr);

        destroyWindow();
    }
}
