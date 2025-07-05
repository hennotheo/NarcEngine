#pragma once

#include <vulkan/vulkan_core.h>

#include "CommandBuffer.h"

namespace narc_engine {
    class CommandPoolRhi
    {
    public:
        CommandPoolRhi();
        ~CommandPoolRhi();

        CommandBuffer* getCommandBuffer(uint32_t index) { return &m_commandBuffers[index]; }

        void createCommandBuffers(uint32_t commandBufferCount);

        CommandBuffer beginSingleTimeCommands() const;
        void endSingleTimeCommands(CommandBuffer commandBuffer) const;

    private:
        VkCommandPool m_commandPool;

        std::vector<CommandBuffer> m_commandBuffers;
    };
}
