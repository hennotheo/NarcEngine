#pragma once

#include <NarcEngine.h>

namespace narc
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void run();

    private:
        narc_engine::Engine* m_engine;
    };
}