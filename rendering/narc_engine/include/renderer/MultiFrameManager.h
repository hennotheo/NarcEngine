//
// Created by theoh on 3/10/2025.
//

#pragma once

#include "FrameHandler.h"

namespace narc_engine {

    class MultiFrameManager
    {
    public:
        explicit MultiFrameManager(uint32_t maxFrameInFlight);
        ~MultiFrameManager();

        void nextFrame();
        GETTER DEPRECATED uint32_t getCurrentFrame() const { return m_currentFrame; }
        GETTER const FrameHandler* getCurrentFrameHandler() const { return m_frames[m_currentFrame].get(); }

    private:
        const uint32_t m_maxFrameInFlight;

        std::vector<std::unique_ptr<FrameHandler>> m_frames;
        uint32_t m_currentFrame = 0;
    };

} // narc_engine