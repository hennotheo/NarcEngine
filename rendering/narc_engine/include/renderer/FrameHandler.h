//
// Created by theoh on 3/10/2025.
//
#pragma once

#include <vulkan/vulkan.h>

#include "platform/vulkan/CommandPool.h"
#include "platform/vulkan/DescriptorPool.h"
#include "buffers/UniformBuffer.h"

#include "platform/vulkan/sync/Semaphore.h"
#include "platform/vulkan/sync/Fence.h"

namespace narc_engine {
    class FrameHandler final
    {
        friend class MultiFrameManager;

    public:
        FrameHandler();
        ~FrameHandler();

        DEPRECATED NARC_GETTER(CommandPool*, getCommandPool, m_commandPool.get());

        NARC_GETTER(const Semaphore*, getImageAvailableSemaphore, m_imageAvailableSemaphore.get());
        NARC_GETTER(const Semaphore*, getRenderFinishedSemaphore, m_renderFinishedSemaphore.get());
        NARC_GETTER(const Fence*, getInFlightFence, m_inFlightFence.get());
        NARC_GETTER(UniformBuffer*, getUniformBuffer, m_uniformBuffer.get());
        NARC_GETTER(const VkDescriptorSet&, getDescriptorSet, m_descriptorSets.at(id), const ResourceId &id);

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
