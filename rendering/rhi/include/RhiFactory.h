//
// Created by theo on 2/18/26.
//

#pragma once

#include "interface/enums/GraphicBackend.h"

namespace narc_engine {
    class IGraphicsInstance;

    std::unique_ptr<IGraphicsInstance> createGraphicsInstance(GraphicBackend backend);
}
