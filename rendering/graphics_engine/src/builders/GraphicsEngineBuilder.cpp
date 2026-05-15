//
// Created by theo on 5/10/26.
//

#include "builders/GraphicsEngineBuilder.h"

#include "engine/NarcGraphicsEngine.h"

namespace narc_engine {

    IGraphicsEngineBuilder& GraphicsEngineBuilder::useVulkan()
    {
        m_useVulkan = true;

        return *this;
    }

    std::unique_ptr<IGraphicsEngine> GraphicsEngineBuilder::build()
    {
        auto instance = createGraphicsInstance(GraphicBackend::Vulkan);

        return std::make_unique<NarcGraphicsEngine>(instance);
    }
}
