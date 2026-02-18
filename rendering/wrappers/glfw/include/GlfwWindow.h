//
// Created by theo on 2/18/26.
//

#pragma once

struct GLFWwindow;

namespace narc_engine {
    class GlfwWindow : public IWindow
    {
    public:
        GlfwWindow();
        ~GlfwWindow() override;

        NARC_IMPL_INITIALISABLE();

        NARC_OVERRIDE_GETTER(void*, getNativeHandle, m_window);

        NARC_QUERY_OVERRIDE(bool, shouldClose);
        NARC_QUERY_OVERRIDE(SurfaceExtend, getSurfaceExtent);

        NARC_OVERRIDE_GETTER(std::string_view, getTitle, m_title)

        void setTitle(const std::string& value) noexcept override;

    private:
        GLFWwindow* m_window = nullptr;
        bool m_isOpen = false;

        std::string m_title;
    };
}
