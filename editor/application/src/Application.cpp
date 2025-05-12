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
        m_transforms.push_back(new narc_math::Transform());
        m_transforms.push_back(new narc_math::Transform());
        m_transforms.push_back(new narc_math::Transform());
        const narc_io::Model3D model = narc_io::FileReader::load3DModel(g_modelPath);
        const narc_io::Model3D model2 = narc_io::FileReader::load3DModel(g_model2Path);

        narc_engine::ResourceId mat = narc_engine::createMaterial(g_texturePath.c_str());
        // m_materials.push_back(new narc_engine::Material(g_texturePath.c_str()));
        // m_materials.push_back(new narc_engine::Material(g_texture2Path.c_str()));
        
        m_renderers.push_back(new narc_engine::Renderer(&model, mat, m_transforms[0]));
        m_renderers.push_back(new narc_engine::Renderer(&model2, mat, m_transforms[1]));
        m_renderers.push_back(new narc_engine::Renderer(&model, mat, m_transforms[2]));

        m_engine->binder()->attachGuiComponent(&m_testWindow);

        m_transforms[2]->setWorldScale(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    void Application::stop()
    {
        m_engine->waitDeviceIdle();

        for (auto& renderer : m_renderers)
        {
            delete renderer;
        }
        m_renderers.clear();

        for (auto& transform : m_transforms)
        {
            delete transform;
        }
        m_transforms.clear();
    }

    void Application::appLoopBody()
    {
        // PRE-UPDATE ENGINE LOGIC
        m_engine->pollEvents();

        double newTime = 0;//m_engine->window()->getWindowTime();
        double deltaTime = 0.0166666666666667;
        m_currentTime += deltaTime;

        // UPDATE ENGINE LOGIC
        m_transforms[0]->rotateZ(10.0 * deltaTime);
        m_transforms[1]->rotateZ(-5.0 * deltaTime);
        m_transforms[2]->rotateY(-15.0 * deltaTime);
        m_transforms[2]->setWorldPosition(glm::vec3(0.5f, 0.0f, glm::sin(m_currentTime / 10.0) * 0.5));

        // RENDER ENGINE
        m_engine->render();
    }
}
