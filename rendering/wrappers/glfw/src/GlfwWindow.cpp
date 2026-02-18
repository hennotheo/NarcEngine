//
// Created by theo on 2/18/26.
//

#include "GlfwWindow.h"

#include "GLFW/glfw3.h"

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
