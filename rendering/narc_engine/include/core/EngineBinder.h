#pragma once

namespace narc_io
{
    struct Image;
}

namespace narc_engine
{
    class Renderer;
    class IEngine;

    class NARC_ENGINE_API EngineBinder
    {
    public:
        EngineBinder(IEngine* engine) : m_engine(engine)
        {
        }

        void attachRenderer(const Renderer* renderer) const;

    private:
        IEngine* m_engine;
    };
} // narc_engine
