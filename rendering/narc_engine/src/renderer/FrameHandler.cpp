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
        m_inFlightFence = std::make_unique<Fence>();

        m_uniformBuffer = std::make_unique<UniformBuffer>();
    }

    FrameHandler::~FrameHandler()
    {
        
    }
} // narc_engine
