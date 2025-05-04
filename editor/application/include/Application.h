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

        std::vector<narc_math::Transform*> m_transforms;
        std::vector<const narc_engine::Material*> m_materials;
        std::vector<narc_engine::Renderer*> m_renderers;

        double m_currentTime = 0.0;
    };
}