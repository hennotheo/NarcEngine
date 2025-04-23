//
// Created by theoh on 24/02/2025.
//

#include "Application.h"

#include <NarcIO.h>
#include <NarcLog.h>

namespace narc
{
    const std::string g_modelPath = "models/test.obj";
    const std::string g_model2Path = "models/test2.obj";
    const std::string g_texturePath = "textures/test.png";
    const std::string g_texture2Path = "textures/test2.png";

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
        m_transform = new narc_math::Transform();
        const narc_io::Model3D model = narc_io::FileReader::load3DModel(g_modelPath);
        const narc_io::Model3D model2 = narc_io::FileReader::load3DModel(g_model2Path);
        m_renderMaterial = new narc_engine::Material(g_texturePath.c_str());
        m_renderMaterial2 = new narc_engine::Material(g_texture2Path.c_str());

        m_renderer = new narc_engine::Renderer(&model, m_renderMaterial, m_transform);
        m_renderer2 = new narc_engine::Renderer(&model2, m_renderMaterial2, m_transform);
    }

    void Application::stop()
    {
        m_engine->waitDeviceIdle();

        delete m_transform;
        delete m_renderer;
        delete m_renderer2;
        delete m_mesh;
        delete m_renderMaterial;
        delete m_renderMaterial2;
    }

    void Application::appLoopBody()
    {
        // PRE-UPDATE ENGINE LOGIC
        m_engine->pollEvents();

        double newTime = m_engine->window()->getWindowTime();
        double deltaTime = newTime - m_currentTime;
        m_currentTime = newTime;

        // UPDATE ENGINE LOGIC
        m_transform->rotateZ(50.0 * deltaTime);

        // RENDER ENGINE
        m_engine->render();
    }
}
