//
// Created by theoh on 3/2/2025.
//

#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine {
    class RenderPass: public narc_core::IGetter<VkRenderPass>
    {
    public:
        explicit RenderPass(VkFormat colorFormat);
        ~RenderPass();

        NARC_IMPL_IGETTER(VkRenderPass, m_renderPass);

    private:
        VkRenderPass m_renderPass;
    };
} // narc_engine
