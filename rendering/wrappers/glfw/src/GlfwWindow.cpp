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
        //TODO: TEMP
        glfwPollEvents();
        return glfwWindowShouldClose(m_window);
    }

    SurfaceExtend GlfwWindow::getSurfaceExtent() const noexcept
    {
        int width;
        int height;
        glfwGetFramebufferSize(m_window, &width, &height);

        return {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };
    }

    void GlfwWindow::setTitle(const std::string& value) noexcept
    {
        m_title = value;
    }
}
