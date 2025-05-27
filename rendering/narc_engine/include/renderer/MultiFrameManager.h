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

        NARC_GETTER(DescriptorPool* , getDescriptorPool, m_descriptorPool.get());
        NARC_GETTER(uint32_t, getMaxFrameInFlight, m_maxFrameInFlight);
        NARC_GETTER(const FrameHandler* , getCurrentFrameHandler, m_frames[m_currentFrame].get())

        void nextFrame();
        void allocateDescriptorSets(const std::vector<ResourceId>& setIds, VkDescriptorSetAllocateInfo& allocInfo);

    private:
        const uint32_t m_maxFrameInFlight;

        std::unique_ptr<DescriptorPool> m_descriptorPool;

        std::vector<std::unique_ptr<FrameHandler>> m_frames;
        uint32_t m_currentFrame = 0;
    };
} // narc_engine
