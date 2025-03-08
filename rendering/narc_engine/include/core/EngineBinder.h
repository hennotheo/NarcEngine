#pragma once

namespace narc_io
{
    struct Image;
}

namespace narc_engine
{
    class Material;
    class IEngine;
    class Mesh;

    class NARC_ENGINE_API EngineBinder
    {
    public:
        EngineBinder(IEngine* engine) : m_engine(engine)
        {
        }

        void bindMesh(const Mesh* mesh) const;
        void unbindMesh(const Mesh* mesh) const;

        void bindMaterial(const Material* material) const;

    private:
        IEngine* m_engine;
    };
} // narc_engine
