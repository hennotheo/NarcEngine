//
// Created by theoh on 3/10/2025.
//

#include "renderer/MultiFrameManager.h"

namespace narc_engine {
    MultiFrameManager::MultiFrameManager(const uint32_t maxFrameInFlight) : m_maxFrameInFlight(maxFrameInFlight)
    {
        m_frames = std::vector<std::unique_ptr<FrameHandler> >(maxFrameInFlight);
        for (uint32_t i = 0; i < maxFrameInFlight; ++i)
        {
            m_frames[i] = std::make_unique<FrameHandler>();
        }
    }

    MultiFrameManager::~MultiFrameManager() = default;

    void MultiFrameManager::nextFrame()
    {
        m_currentFrame = (m_currentFrame + 1) % m_maxFrameInFlight;
    }
} // narc_engine
