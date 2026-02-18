//
// Created by theo on 2/18/26.
//

#pragma once

#include "enums/GraphicBackend.h"
#include "enums/WindowBackend.h"

namespace narc_engine {
    class IWindow;
    class IGraphicsInstance;

    std::unique_ptr<IGraphicsInstance> createGraphicsInstance(GraphicBackend backend);

    std::unique_ptr<IWindow> createWindow(WindowBackend backend);
}
