//
// Created by theoh on 3/10/2025.
//

#include "renderer/FrameHandler.h"

#include <NarcLog.h>

#include "Engine.h"

namespace narc_engine {
    FrameHandler::FrameHandler()
    {
        m_device = Engine::getInstance()->getDevice();

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(m_device->getDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(m_device->getDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(m_device->getDevice(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create semaphores!");
        }

        m_uniformBuffer = std::make_unique<UniformBuffer>(sizeof(UniformBufferObject));
    }

    FrameHandler::~FrameHandler()
    {
        vkDestroySemaphore(m_device->getDevice(), m_renderFinishedSemaphore, nullptr);
        vkDestroySemaphore(m_device->getDevice(), m_imageAvailableSemaphore, nullptr);
        vkDestroyFence(m_device->getDevice(), m_inFlightFence, nullptr);
    }
} // narc_engine
