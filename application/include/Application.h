#pragma once

#include <NarcEngine.h>

namespace narc
{
    class Application
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
        const narc_engine::Mesh* m_mesh = nullptr;
    };
}