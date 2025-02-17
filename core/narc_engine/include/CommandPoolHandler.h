#pragma once
#include <vulkan/vulkan_core.h>

#include "devices/DeviceHandler.h"

namespace narc_engine
{
    class CommandPoolHandler
    {
    public:
        VkCommandBuffer getCommandBuffer(uint32_t index) const { return m_commandBuffers[index]; }

        void create(const DeviceHandler* deviceHandler);
        void createCommandBuffers(uint32_t commandBufferCount);

        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        void release();

    private:
        VkCommandPool m_commandPool;

        std::vector<VkCommandBuffer> m_commandBuffers;

        const DeviceHandler* m_deviceHandler;
    };
}
