#pragma once

#include <NarcEngine.h>

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

        narc_engine::Renderer* m_renderer = nullptr;
        narc_engine::Renderer* m_renderer2 = nullptr;
    };
}