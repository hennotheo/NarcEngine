#pragma once
#include <vulkan/vulkan_core.h>

#include "CommandBuffer.h"
#include "core/DeviceHandler.h"

namespace narc_engine
{
    class CommandPool
    {
    public:
        CommandBuffer* getCommandBuffer(uint32_t index) { return &m_commandBuffers[index]; }

        void create(const DeviceHandler* deviceHandler);
        void createCommandBuffers(uint32_t commandBufferCount);

        CommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(CommandBuffer commandBuffer);

        void release();

    private:
        VkCommandPool m_commandPool;

        std::vector<CommandBuffer> m_commandBuffers;

        const DeviceHandler* m_deviceHandler;
    };
}
