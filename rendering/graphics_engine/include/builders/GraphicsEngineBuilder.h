//
// Created by theo on 5/10/26.
//


#pragma once

#include <NarcGraphicsEngine.h>

namespace narc_engine {
    class GraphicsEngineBuilder : public IGraphicsEngineBuilder
    {
    public:
        IGraphicsEngineBuilder& useVulkan() override;
        std::unique_ptr<IGraphicsEngine> build() override;

    private:
        bool m_useVulkan = false;
    };
}
