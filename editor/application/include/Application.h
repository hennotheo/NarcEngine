#pragma once

namespace narc
{
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

        std::vector<narc_math::Transform*> m_transforms;
        std::vector<narc_engine::Renderer*> m_renderers;

        double m_currentTime = 0.0;
    };
}