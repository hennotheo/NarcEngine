//
// Created by theoh on 27/05/2025.
//
#include "WindowRhi.h"

#define GLFW_INCLUDE_VULKAN
#include "ContextRhi.h"
#include "GLFW/glfw3.h"
#include "backend_vulkan/WindowVulkan.h"

namespace narc_engine
{
    WindowRhiPtr createWindowRhi(RendererApiType api, const ContextRhi* ctx)
    {
        switch (api)
        {
        case RendererApiType::Vulkan:
            return std::make_unique<WindowVulkan>(ctx->getContextVulkan());

        case RendererApiType::OpenGL:
            NARC_FATAL_OPENGL_NOT_SUPPORTED();

        case RendererApiType::DirectX12:
            NARC_FATAL_DIRECTX12_NOT_SUPPORTED();

        default:
            break;
        }
        return nullptr;
    }

    WindowRhi::WindowRhi(const ContextRhi* ctx) :
        narc_core::IInitialisable(), m_context(ctx)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    }

    WindowRhi::~WindowRhi()
    {
        glfwTerminate();
    }

    void WindowRhi::createWindow()
    {
        m_window = glfwCreateWindow(800, 600, "Narc Engine RHI Window", nullptr, nullptr);
        if (!m_window)
        {
            NARCLOG_FATAL("Failed to create GLFW window!");
        }

        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
        glfwSetKeyCallback(m_window, onKeyboardInputPerformed);
        glfwSetMouseButtonCallback(m_window, onMouseInputPerformed);
    }

    void WindowRhi::destroyWindow()
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    void WindowRhi::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        WindowRhi* windowObject = reinterpret_cast<WindowRhi*>(glfwGetWindowUserPointer(window));
        windowObject->m_onFramebufferResized.trigger(width, height);

        windowObject->m_framebufferResized = true;
    }

    void WindowRhi::onKeyboardInputPerformed(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
    {
        WindowRhi* windowObject = reinterpret_cast<WindowRhi*>(glfwGetWindowUserPointer(window));
        windowObject->m_onKeyboardEvent.trigger(key, scancode, action, mods);
    }

    void WindowRhi::onMouseInputPerformed(GLFWwindow* window, const int button, const int action, const int mods)
    {
        WindowRhi* windowObject = reinterpret_cast<WindowRhi*>(glfwGetWindowUserPointer(window));
        windowObject->m_onMouseEvent.trigger(button, action, mods);
    }
}
