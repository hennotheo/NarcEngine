//
// Created by theo on 5/14/26.
//

#include "engine/NarcGraphicsEngine.h"

namespace narc_engine {

    NarcGraphicsEngine::NarcGraphicsEngine(std::unique_ptr<IGraphicsInstance>& graphicsInstance) :
        m_graphicsInstance(std::move(graphicsInstance))
    {
    }

    NarcGraphicsEngine::~NarcGraphicsEngine() = default;

    void NarcGraphicsEngine::init()
    {
        m_graphicsInstance->init();
    }

    void NarcGraphicsEngine::shutdown()
    {
        m_graphicsInstance->shutdown();
    }
}
