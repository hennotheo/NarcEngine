//
// Created by theoh on 3/10/2025.
//

#include "renderer/FrameHandler.h"

#include <NarcLog.h>

#include "Engine.h"

namespace narc_engine {
    FrameHandler::FrameHandler() : DeviceComponent()
    {
        m_commandPool = std::make_unique<CommandPool>();
        m_commandPool->createCommandBuffers(10);

        m_renderFinishedSemaphore = std::make_unique<Semaphore>();
        m_imageAvailableSemaphore = std::make_unique<Semaphore>();

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(getVkDevice(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create semaphores!");
        }

        m_uniformBuffer = std::make_unique<UniformBuffer>();
    }

    FrameHandler::~FrameHandler()
    {
        vkDestroyFence(getVkDevice(), m_inFlightFence, nullptr);
    }
} // narc_engine
