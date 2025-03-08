//
// Created by theoh on 24/02/2025.
//

#include "Application.h"

#include <NarcIO.h>
#include <NarcLog.h>

namespace narc {
    const std::string g_modelPath = "models/test.obj";
    const std::string g_texturePath = "textures/test.png";

    Application::Application()
    {
        m_engine = narc_engine::createEngine();
    }

    Application::~Application()
    {
        delete m_engine;
    }

    bool Application::shouldClose() const
    {
        return m_engine->shouldClose();
    }

    void Application::start()
    {
        m_renderMaterial = new narc_engine::Material(g_texturePath.c_str());
        m_engine->binder()->bindMaterial(m_renderMaterial);

        std::vector<narc_engine::Vertex> vertices;
        std::vector<uint32_t> indices;

        narc_io::Model3D model = narc_io::FileReader::load3DModel(g_modelPath);
        const narc_io::VertexList modelVertices = *model.getVertices();
        const narc_io::TexCoordList modelTexCoords = *model.getTexCoords();
        const narc_io::IndexList modelIndices = *model.getIndices();
        for (int i = 0; i < model.getVerticesCount(); ++i)
        {
            vertices.push_back({
                {modelVertices[i][0], modelVertices[i][1], modelVertices[i][2]},
                {1.0f, 1.0f, 1.0f},
                {modelTexCoords[i][0], modelTexCoords[i][1]}
            });
        }
        for (uint32_t modelIndice: modelIndices)
        {
            indices.push_back(modelIndice);
        }

        m_mesh = new narc_engine::Mesh(vertices, indices);
        narc_engine::getEngine()->binder()->bindMesh(m_mesh);

    }

    void Application::stop()
    {
        m_engine->waitDeviceIdle();

        delete m_mesh;
        delete m_renderMaterial;
    }

    void Application::appLoopBody()
    {
        //PRE-UPDATE ENGINE LOGIC
        m_engine->pollEvents();

        //UPDATE ENGINE LOGIC

        //RENDER ENGINE
        m_engine->render();
    }
}
