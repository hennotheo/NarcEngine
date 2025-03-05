//
// Created by theoh on 3/2/2025.
//

#pragma once

#include <vulkan/vulkan_core.h>

namespace narc_engine {
    class DeviceHandler;

    class DepthResources final
    {
    public:
        DepthResources();
        ~DepthResources();

        VkImageView getImageView() const { return m_depthImageView; }

        void create(uint32_t width, uint32_t height);
        void release();

    private:
        VkImage m_depthImage;
        VkDeviceMemory m_depthImageMemory;
        VkImageView m_depthImageView;

        const DeviceHandler* m_deviceHandler = nullptr;
    };
}
