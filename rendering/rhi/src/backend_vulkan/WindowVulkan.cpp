//
// Created by theoh on 27/05/2025.
//
#include "backend_vulkan/WindowVulkan.h"

#include <GLFW/glfw3.h>

#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    WindowVulkan::WindowVulkan(const ContextVulkan* ctx):
        WindowRhi(ctx)
    {

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
