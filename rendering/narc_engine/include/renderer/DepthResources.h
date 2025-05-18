//
// Created by theoh on 3/2/2025.
//

#pragma once

#include <vulkan/vulkan_core.h>

#include "platform/vulkan/ImageView.h"
#include "platform/vulkan/DeviceMemory.h"

namespace narc_engine {
    class DeviceHandler;

    class DepthResources final
    {
    public:
        DepthResources() = default;
        ~DepthResources();

        const ImageView* getImageView() const { return &m_depthImageView; }

        void create(uint32_t width, uint32_t height);
        void release();

    private:
        VkImage m_depthImage;
        DeviceMemory m_depthImageMemory;
        ImageView m_depthImageView;
    };
}
