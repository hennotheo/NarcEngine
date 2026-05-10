//
// Created by theo on 5/10/26.
//

#pragma once

namespace narc_engine {
    class IGraphicsPipeline;
    class IWindow;
    class IGraphicsInstance;

    struct GuiInitContext
    {
        IGraphicsInstance* GraphicsInstance;
        IWindow* window;
        IGraphicsPipeline* Pipeline;

        uint32_t ImageCount;
        uint32_t MinImageCount;
    };
}