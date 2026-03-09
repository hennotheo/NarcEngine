//
// Created by theo on 2/18/26.
//

#include "GlfwWindow.h"

#include <X11/Xlib-xcb.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace narc_engine {
    GlfwWindow::GlfwWindow() = default;

    GlfwWindow::~GlfwWindow() = default;

    void GlfwWindow::init()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_window = glfwCreateWindow(640, 480, getTitle().data(), NULL, NULL);
        if (m_window == nullptr)
        {
            NARC_ERROR_RUNTIME("failed to create window surface!");
        }

        const auto display = glfwGetX11Display();
        const auto window = glfwGetX11Window(m_window);

        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetWindowUserPointer(m_window, this);

        m_handles = NativeWindowHandle{};
        m_handles.Connection = XGetXCBConnection(display);
        m_handles.Window = static_cast<xcb_window_t>(window);

        m_isOpen = true;

        NARC_LOG_DEBUG("GlfwWindow Initialized.");
    }

    void GlfwWindow::shutdown()
    {
        glfwDestroyWindow(m_window);
        NARC_LOG_DEBUG("GlfwWindow destroyed successfully.");

        m_isOpen = false;
    }

    bool GlfwWindow::shouldClose() const noexcept
    {
        return glfwWindowShouldClose(m_window);
    }

    SurfaceExtent GlfwWindow::getSurfaceExtent() const noexcept
    {
        int width;
        int height;
        glfwGetFramebufferSize(m_window, &width, &height);

        return {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };
    }

    void GlfwWindow::update()
    {
        //Reset then poll
        m_framebufferResized = false;

        glfwPollEvents();
    }

    void GlfwWindow::setTitle(const std::string& value) noexcept
    {
        m_title = value;
    }

    void GlfwWindow::framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height)
    {
        const auto window = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(glfwWindow));
        window->m_framebufferResized = true;
        NARC_LOG_DEBUG("GlfwWindow framebuffer resized.");
    }

    void GlfwWindow::mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
    {
        const auto window = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(glfwWindow));
        NARC_LOG_DEBUG("GlfwWindow Mouse but.{} {} {}", button, action, mods);
    }
}
