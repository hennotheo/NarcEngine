//
// Created by theoh on 3/2/2025.
//

#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"

namespace narc_engine {
    class DeviceHandler;

    class RenderPass : public DeviceComponent
    {
    public:
        explicit RenderPass(VkFormat colorFormat);
        ~RenderPass();

        const VkRenderPass& getRenderPass() const { return m_renderPass; }

    private:
        VkRenderPass m_renderPass;
    };
} // narc_engine
