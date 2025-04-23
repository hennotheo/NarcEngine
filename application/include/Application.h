#pragma once

#include <NarcEngine.h>
#include <NarcMath.h>

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

        const narc_engine::Material* m_renderMaterial = nullptr;
        const narc_engine::Material* m_renderMaterial2 = nullptr;
        const narc_engine::Mesh* m_mesh = nullptr;

        narc_math::Transform* m_transform;

        narc_engine::Renderer* m_renderer = nullptr;
        narc_engine::Renderer* m_renderer2 = nullptr;

        double m_currentTime = 0.0;
    };
}