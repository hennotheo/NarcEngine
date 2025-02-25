#include "CommandPool.h"

#include "core/DeviceHandler.h"

namespace narc_engine
{
    void CommandPool::create(const DeviceHandler* deviceHandler)
    {
        m_deviceHandler = deviceHandler;

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        m_deviceHandler->createCommandPool(&m_commandPool, poolInfo);
    }

    void CommandPool::release()
    {
        vkDestroyCommandPool(m_deviceHandler->getDevice(), m_commandPool, nullptr);
    }

    void CommandPool::createCommandBuffers(uint32_t commandBufferCount)
    {
        if (!m_commandBuffers.empty())
        {
            throw std::runtime_error("Command buffers have already been created!");
        }

        m_commandBuffers.resize(commandBufferCount);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPool;
        allocInfo.commandBufferCount = commandBufferCount;

        CommandBuffer::allocateBuffers(m_deviceHandler, &allocInfo, m_commandBuffers);
    }

    CommandBuffer CommandPool::beginSingleTimeCommands()
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
        commandBuffer.allocate(m_deviceHandler, &allocInfo);
        commandBuffer.begin(beginInfo);

        return commandBuffer;
    }

    void CommandPool::endSingleTimeCommands(CommandBuffer commandBuffer)
    {
        commandBuffer.end();

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffer.getVkCommandBuffer();

        m_deviceHandler->submitGraphicsQueue(1, &submitInfo, VK_NULL_HANDLE);
        m_deviceHandler->waitGraphicsQueueIdle();

        commandBuffer.release(m_deviceHandler, m_commandPool);
    }
}
