//
// Created by theoh on 3/10/2025.
//
#pragma once

#include <vulkan/vulkan.h>

#include "CommandPool.h"
#include "DescriptorPool.h"
#include "buffers/UniformBuffer.h"

#include "sync/Semaphore.h"
#include "sync/Fence.h"

namespace narc_engine {
    class DeviceHandler;

    class FrameHandler : public DeviceComponent
    {
        friend class MultiFrameManager;

    public:
        FrameHandler();
        ~FrameHandler();

        DEPRECATED GETTER CommandPool* getCommandPool() const { return m_commandPool.get(); }

        GETTER const Semaphore* getImageAvailableSemaphore() const { return m_imageAvailableSemaphore.get(); }
        GETTER const Semaphore* getRenderFinishedSemaphore() const { return m_renderFinishedSemaphore.get(); }
        GETTER const Fence* getInFlightFence() const { return m_inFlightFence.get(); }

        GETTER UniformBuffer* getUniformBuffer() const { return m_uniformBuffer.get(); }
        GETTER const VkDescriptorSet& getDescriptorSet(ResourceId id) const { return m_descriptorSets.at(id); }

    private:
        std::unique_ptr<CommandPool> m_commandPool;

        std::unique_ptr<Semaphore> m_imageAvailableSemaphore;
        std::unique_ptr<Semaphore> m_renderFinishedSemaphore;
        std::unique_ptr<Fence> m_inFlightFence;

        std::unique_ptr<UniformBuffer> m_uniformBuffer;

        std::unordered_map<ResourceId, VkDescriptorSet> m_descriptorSets;

    private:
        void addDescriptorSets(ResourceId id, const VkDescriptorSet descriptorSet);
    };
} // narc_engine
