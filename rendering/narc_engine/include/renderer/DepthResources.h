//
// Created by theoh on 3/2/2025.
//

#pragma once

#include <vulkan/vulkan_core.h>

#include "core/DeviceComponent.h"
#include "resources/ImageView.h"

namespace narc_engine {
    class DeviceHandler;

    class DepthResources final : public DeviceComponent
    {
    public:
        DepthResources() = default;
        ~DepthResources();

        VkImageView getImageView() const { return m_depthImageView.getVkImageView(); }

        void create(uint32_t width, uint32_t height);
        void release();

    private:
        VkImage m_depthImage;
        VkDeviceMemory m_depthImageMemory;
        ImageView m_depthImageView;
    };
}
