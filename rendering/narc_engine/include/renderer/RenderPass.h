//
// Created by theoh on 3/2/2025.
//

#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine {
    class DeviceHandler;

    class RenderPass
    {
    public:
        explicit RenderPass(VkFormat colorFormat);
        ~RenderPass();

        const VkRenderPass& getRenderPass() const { return m_renderPass; }

    private:
        VkRenderPass m_renderPass;

        const DeviceHandler* m_deviceHandler = nullptr;
    };
} // narc_engine
