//
// Created by theo on 2/18/26.
//

#pragma once
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace narc_engine {
    class GlfwWindow : public IWindow
    {
    public:
        GlfwWindow();
        ~GlfwWindow() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(NativeWindowHandle, getNativeHandle, m_handles);

        NARC_QUERY_OVERRIDE(bool, shouldClose);
        NARC_QUERY_OVERRIDE(SurfaceExtent, getSurfaceExtent);

        NARC_OVERRIDE_GETTER(std::string_view, getTitle, m_title);
        NARC_OVERRIDE_GETTER(bool, isResizing, m_framebufferResized);

        void update() override;

        void setTitle(const std::string& value) noexcept override;

    private:
        GLFWwindow* m_window = nullptr;
        NativeWindowHandle m_handles{};

        bool m_isOpen = false;
        bool m_framebufferResized = false;

        std::string m_title;

        static void framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);
        static void mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
    };
}
