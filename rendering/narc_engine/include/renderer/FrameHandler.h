//
// Created by theoh on 3/10/2025.
//
#pragma once

#include <vulkan/vulkan.h>

#include "CommandPool.h"
#include "DescriptorPool.h"
#include "buffers/UniformBuffer.h"

namespace narc_engine {
    class DeviceHandler;

    class FrameHandler : public DeviceComponent
    {
        friend class MultiFrameManager;

    public:
        FrameHandler();
        ~FrameHandler();

        DEPRECATED GETTER CommandPool* getCommandPool() const { return m_commandPool.get(); }

        GETTER VkSemaphore getImageAvailableSemaphore() const { return m_imageAvailableSemaphore; }
        GETTER VkSemaphore getRenderFinishedSemaphore() const { return m_renderFinishedSemaphore; }
        GETTER VkFence getInFlightFence() const { return m_inFlightFence; }

        GETTER UniformBuffer* getUniformBuffer() const { return m_uniformBuffer.get(); }
        GETTER const std::vector<VkDescriptorSet>& getDescriptorSets() const { return m_descriptorSets; }

    private:
        std::unique_ptr<CommandPool> m_commandPool;

        VkSemaphore m_imageAvailableSemaphore;
        VkSemaphore m_renderFinishedSemaphore;
        VkFence m_inFlightFence;

        std::unique_ptr<UniformBuffer> m_uniformBuffer;

        std::vector<VkDescriptorSet> m_descriptorSets;

        void addDescriptorSets(const VkDescriptorSet descriptorSet) { m_descriptorSets.push_back(descriptorSet); }
    };
} // narc_engine
