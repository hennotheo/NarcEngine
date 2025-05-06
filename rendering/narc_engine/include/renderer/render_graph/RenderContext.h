#pragma once

#include <vulkan/vulkan.h>

#include "models/Renderer.h"

namespace narc_engine
{
    struct RenderContext
    {
        VkExtent2D SwapChainExtent;
        
        const Renderer** Renderers;
        uint32_t RenderersCount;

        const std::vector<VkDescriptorSet>* DescriptorSets;
    };
} // namespace narc_engine
