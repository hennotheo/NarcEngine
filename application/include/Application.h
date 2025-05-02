#pragma once

#include <NarcEngine.h>
#include <NarcMath.h>

namespace narc
{
    class TestWindow : public narc_engine::GuiWindow
    {
    public:
        TestWindow() : GuiWindow("Test Window") {}
        ~TestWindow() override = default;

    protected:
        inline void renderContent() const override final
        {
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

        narc_engine::Renderer* m_renderer = nullptr;
        narc_engine::Renderer* m_renderer2 = nullptr;

        double m_currentTime = 0.0;
    };
}