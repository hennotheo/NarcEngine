//
// Created by theoh on 3/10/2025.
//
#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine {
    class DeviceHandler;

    class FrameHandler
    {
    public:
        FrameHandler();
        ~FrameHandler();

        GETTER VkSemaphore getImageAvailableSemaphore() const { return m_imageAvailableSemaphore; }
        GETTER VkSemaphore getRenderFinishedSemaphore() const { return m_renderFinishedSemaphore; }
        GETTER VkFence getInFlightFence() const { return m_inFlightFence; }

    private:
        VkSemaphore m_imageAvailableSemaphore;
        VkSemaphore m_renderFinishedSemaphore;
        VkFence m_inFlightFence;

        const DeviceHandler* m_device = nullptr;
    };
} // narc_engine
