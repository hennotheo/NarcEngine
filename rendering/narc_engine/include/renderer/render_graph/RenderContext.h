#pragma once

#include <vulkan/vulkan.h>

#include "renderer/FrameHandler.h"
#include "models/Renderer.h"

namespace narc_engine
{
    struct RenderContext
    {
        VkExtent2D SwapChainExtent;

        const std::vector<const Renderer*>* Renderers;
        uint32_t RenderersCount;

#pragma warning TODO: remove this temporary code, this is just for testing purposes
        const FrameHandler* FrameHandler;
    };
} // namespace narc_engine
