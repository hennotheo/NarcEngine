//
// Created by theoh on 3/10/2025.
//

#pragma once

#include "FrameHandler.h"

namespace narc_engine
{
    class MultiFrameManager
    {
    public:
        explicit MultiFrameManager(uint32_t maxFrameInFlight);
        ~MultiFrameManager();

        GETTER const DescriptorPool *getDescriptorPool() const { return m_descriptorPool.get(); }
        GETTER const uint32_t getMaxFrameInFlight() const { return m_maxFrameInFlight; }
        GETTER const FrameHandler *getCurrentFrameHandler() const { return m_frames[m_currentFrame].get(); }

        void nextFrame();
        void allocateDescriptorSets(VkDescriptorSetAllocateInfo &allocInfo);

    private:
        const uint32_t m_maxFrameInFlight;

        std::unique_ptr<DescriptorPool> m_descriptorPool;

        std::vector<std::unique_ptr<FrameHandler>> m_frames;
        uint32_t m_currentFrame = 0;
    };
} // narc_engine
