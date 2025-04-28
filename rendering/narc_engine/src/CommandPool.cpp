#include "CommandPool.h"

#include "Engine.h"

namespace narc_engine {
    CommandPool::CommandPool() : DeviceComponent()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        getDeviceHandler()->createCommandPool(&m_commandPool, poolInfo);
    }

    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(getVkDevice(), m_commandPool, nullptr);
    }

    void CommandPool::createCommandBuffers(const uint32_t commandBufferCount)
    {
        if (!m_commandBuffers.empty())
        {
            NARCLOG_FATAL("Command buffers have already been created!");
        }

        m_commandBuffers.resize(commandBufferCount);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool;
        allocInfo.commandBufferCount = commandBufferCount;

        CommandBuffer::allocateBuffers(getDeviceHandler(), &allocInfo, m_commandBuffers);
    }

    CommandBuffer CommandPool::beginSingleTimeCommands() const
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        CommandBuffer commandBuffer;
        commandBuffer.allocate(getDeviceHandler(), &allocInfo);
        commandBuffer.begin(beginInfo);

        return commandBuffer;
    }

    void CommandPool::endSingleTimeCommands(CommandBuffer commandBuffer) const
    {
        commandBuffer.end();

        const std::array<VkCommandBuffer, 1> commandBuffers = {commandBuffer.getVkCommandBuffer()};
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffers.data();

        getDeviceHandler()->submitGraphicsQueue(1, &submitInfo, VK_NULL_HANDLE);
        getDeviceHandler()->waitGraphicsQueueIdle();

        commandBuffer.release(getDeviceHandler(), m_commandPool);
    }
}
