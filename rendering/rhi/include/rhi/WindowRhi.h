//
// Created by theoh on 27/05/2025.
//

#pragma once

struct GLFWwindow;

namespace narc_engine
{
    class ContextRhi;

    NARC_DECL_RHI_PLATFORM_TYPES(Window)

    class NARC_ENGINE_API WindowRhi : public narc_core::IInitialisable
    {
    public:
        explicit WindowRhi(const ContextRhi& ctx);
        ~WindowRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(Window);

        NARC_GETTER(const glm::u32vec2&, getFramebufferSize, m_framebufferSize);

    protected:
        void createWindow();
        void destroyWindow();

    PROTECTED_TESTABLE:
        GLFWwindow* m_window = nullptr;

        const ContextRhi& m_context;

    private:
        bool m_framebufferResized = false;
        glm::i32vec2 m_framebufferSize;

        narc_core::Event<int, int> m_onFramebufferResized;
        narc_core::Event<int, int, int, int> m_onKeyboardEvent;
        narc_core::Event<int, int, int> m_onMouseEvent;

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        static void onKeyboardInputPerformed(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void onMouseInputPerformed(GLFWwindow* window, int button, int action, int mods);
    };

    NARC_DECL_RHI_CREATION(WindowRhi, const ContextRhi& ctx);//TODO : Remove this line when the RhiInjector is ready
}
