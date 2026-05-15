//
// Created by theo on 5/10/26.
//

#include "NarcGraphicsEngine.h"

#include "builders/GraphicsEngineBuilder.h"

namespace narc_engine {
    std::unique_ptr<IGraphicsEngineBuilder> createGraphicsEngineBuilder()
    {
        return std::make_unique<GraphicsEngineBuilder>();
    }
}
