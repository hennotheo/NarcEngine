//
// Created by theo on 5/14/26.
//


#pragma once

#include "interfaces/IGraphicsEngine.h"

namespace narc_engine {
    class RenderGraph;

    class NarcGraphicsEngine : public IGraphicsEngine
    {
    public:
        explicit NarcGraphicsEngine(std::unique_ptr<IGraphicsInstance>& graphicsInstance);
        ~NarcGraphicsEngine() override;

        NARC_IMPL_INITIALISABLE();

    private:
        std::unique_ptr<IGraphicsInstance> m_graphicsInstance = nullptr;
        std::unique_ptr<RenderGraph> m_renderGraph;

        std::unique_ptr<IImage> m_image;
    };
}
