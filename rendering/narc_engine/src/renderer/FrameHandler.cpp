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
        m_commandPool->createCommandBuffers(6);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(getVkDevice(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create semaphores!");
        }

        m_uniformBuffer = std::make_unique<UniformBuffer>(sizeof(UniformBufferObject));
    }

    FrameHandler::~FrameHandler()
    {
        vkDestroySemaphore(getVkDevice(), m_renderFinishedSemaphore, nullptr);
        vkDestroySemaphore(getVkDevice(), m_imageAvailableSemaphore, nullptr);
        vkDestroyFence(getVkDevice(), m_inFlightFence, nullptr);
    }
} // narc_engine
