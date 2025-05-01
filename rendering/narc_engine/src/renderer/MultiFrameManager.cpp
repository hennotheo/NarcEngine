//
// Created by theoh on 3/10/2025.
//

#include "renderer/MultiFrameManager.h"

#include "NarcLog.h"

namespace narc_engine
{
    MultiFrameManager::MultiFrameManager(const uint32_t maxFrameInFlight) : m_maxFrameInFlight(maxFrameInFlight)
    {
        m_descriptorPool = std::make_unique<DescriptorPool>(maxFrameInFlight);
        m_descriptorPool->builder()
            ->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000)
            ->addPoolSize(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, 1000)
            ->setFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
            ->setMaxSet(1000);
        m_descriptorPool->create();

        m_frames = std::vector<std::unique_ptr<FrameHandler>>(maxFrameInFlight);
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

    void MultiFrameManager::allocateDescriptorSets(VkDescriptorSetAllocateInfo &allocInfo)
    {
        const uint32_t descriptorSetCount = allocInfo.descriptorSetCount;
        std::vector<VkDescriptorSet> descriptorSets(descriptorSetCount);
        for (uint32_t i = 0; i < m_maxFrameInFlight; ++i)
        {
            // descriptorSets.clear();//TODO check if this is needed

            m_descriptorPool->allocateDescriptorSets(&allocInfo, descriptorSets.data());
            for (uint32_t j = 0; j < descriptorSetCount; ++j)
            {
                m_frames[i]->addDescriptorSets(descriptorSets[j]);
            }
        }
    }
} // narc_engine
