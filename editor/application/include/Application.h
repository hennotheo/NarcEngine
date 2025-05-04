#pragma once

#include <NarcEngine.h>
#include <NarcMath.h>

namespace narc
{
    class TestWindow : public narc_gui::GuiWindow
    {
    public:
        TestWindow() : GuiWindow("Test Window") {
            setPosition(0.0f, 0.0f);
            setSize(300.0f, 200.0f);
        }
        ~TestWindow() override = default;

    protected:
        inline void renderContent() const override final
        {
            narc_gui::text("Hello, world!");
            narc_gui::text("This is a test window.");
            narc_gui::text("You can add more content here.");
            narc_gui::text("This is a test of the GUI system.");
        }
    };

    class Application final
    {
    public:
        Application();
        virtual ~Application();

        bool shouldClose() const;
        void start();
        void appLoopBody();
        void stop();

    private:
        narc_engine::IEngine* m_engine;
        TestWindow m_testWindow;

        const narc_engine::Material* m_renderMaterial = nullptr;
        const narc_engine::Material* m_renderMaterial2 = nullptr;

        narc_math::Transform* m_transform;
        narc_math::Transform* m_transform2;

        narc_engine::Renderer* m_renderer = nullptr;
        narc_engine::Renderer* m_renderer2 = nullptr;

        double m_currentTime = 0.0;
    };
}