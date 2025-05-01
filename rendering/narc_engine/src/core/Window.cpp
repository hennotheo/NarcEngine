#include "core/Window.h"

#include <NarcLog.h>
#include <NarcCore.h>

#include "core/EngineInstance.h"
#include "core/interfaces/IEngineCallbacks.h"

#include <backends/imgui_impl_glfw.h>

namespace narc_engine
{
    constexpr uint32_t g_width = 800;
    constexpr uint32_t g_height = 600;

    Window::Window(const EngineInstance* engineInstance, IEngineCallbacks* engine) : m_engine(engine)
    {
        m_engineInstance = engineInstance;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_window = glfwCreateWindow(g_width, g_height, "Narc Engine", nullptr, nullptr);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
        glfwSetKeyCallback(m_window, onKeyboardInputPerformed);
        glfwSetMouseButtonCallback(m_window, onMouseInputPerformed);

        if (glfwCreateWindowSurface(m_engineInstance->getvkInstance(), m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create window surface!");
        }
    }

    Window::~Window()
    {
        vkDestroySurfaceKHR(m_engineInstance->getvkInstance(), m_surface, nullptr);

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::pollEvents()
    {
        glfwPollEvents();

        bool shouldClose = glfwWindowShouldClose(m_window);
        if (shouldClose)
        {
            m_engine->stop();
        }

        glfwGetCursorPos(m_window, &m_mouseXpos, &m_mouseYpos);
        m_time = glfwGetTime();
    }

    const char** Window::getRequiredInstanceExtensions(uint32_t* glfwExtensionCount)
    {
        return glfwGetRequiredInstanceExtensions(glfwExtensionCount);
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

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->notifyFramebufferResized(width, height);
    }

    void Window::onKeyboardInputPerformed(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_onKeyboardEvent.trigger(key, scancode, action, mods);
    }

    void Window::onMouseInputPerformed(GLFWwindow* window, int button, int action, int mods)
    {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_onMouseEvent.trigger(button, action, mods);
    }
}