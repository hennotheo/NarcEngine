#pragma once

namespace narc_engine
{
    class Engine;
    class Mesh;

    class NARC_ENGINE_API EngineBinder
    {
    public:
        EngineBinder(Engine* engine) : m_engine(engine)
        {
        }

        void bindMesh(const Mesh* mesh);
        void unbindMesh(const Mesh* mesh);

    private:
        Engine* m_engine;
    };
} // narc_engine
