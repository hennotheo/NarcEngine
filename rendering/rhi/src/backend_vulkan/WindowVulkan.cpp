//
// Created by theoh on 27/05/2025.
//
#include "backend_vulkan/WindowVulkan.h"

#include <GLFW/glfw3.h>

#include <utility>

#include "backend_vulkan/ContextVulkan.h"

namespace narc_engine
{
    std::vector<const char*> getVulkanGLFWRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** extensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        return std::vector<const char*>(extensions, extensions + glfwExtensionCount);
    }

    WindowVulkan::WindowVulkan(const ContextRhiPtr& ctx, const DeviceRhiPtr& device):
        WindowRhi(ctx), m_swapChain(std::make_shared<SwapChainVulkan>(this, device))
    {
        if (!glfwVulkanSupported())
        {
            NARCLOG_FATAL("Vulkan is not supported by GLFW!");
        }
    }

    WindowVulkan::~WindowVulkan() = default;

    void WindowVulkan::init()
    {
        NARC_GUARD_WEAK(context, m_context, "ContextVulkan is null!");

        createWindow();

        if (glfwCreateWindowSurface(context->getContextVulkan()->getVkInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create window surface!");
        }

        if (m_swapChain != nullptr)
        {
            m_swapChain->init();
        }
    }

    void WindowVulkan::shutdown()
    {
        if (m_swapChain != nullptr)
        {
            m_swapChain->shutdown();
        }

        NARC_GUARD_WEAK(context, m_context, "ContextVulkan is null!");

        vkDestroySurfaceKHR(context->getContextVulkan()->getVkInstance(), m_surface, nullptr);

        destroyWindow();
    }
}
