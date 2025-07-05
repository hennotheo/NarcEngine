#include "platform/vulkan/CommandPoolRhi.h"

#include "Engine.h"

namespace narc_engine {
    CommandPoolRhi::CommandPoolRhi()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        QueueFamilyIndices queueFamilyIndices = NARC_PHYSICAL_DEVICE->getQueueFamilyIndices();

        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(NARC_DEVICE_HANDLE, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create command pool!");
        }
    }

    CommandPoolRhi::~CommandPoolRhi()
    {
        vkDestroyCommandPool(NARC_DEVICE_HANDLE, m_commandPool, nullptr);
    }

    void CommandPoolRhi::createCommandBuffers(const uint32_t commandBufferCount)
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

        CommandBuffer::allocateBuffers(&allocInfo, m_commandBuffers);
    }

    CommandBuffer CommandPoolRhi::beginSingleTimeCommands() const
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
        commandBuffer.allocate(&allocInfo);
        commandBuffer.begin(beginInfo);

        return commandBuffer;
    }

    void CommandPoolRhi::endSingleTimeCommands(CommandBuffer commandBuffer) const
    {
        commandBuffer.end();

        const std::array<VkCommandBuffer, 1> commandBuffers = {commandBuffer.getVkCommandBuffer()};
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffers.data();

        const GraphicsQueue* graphicsQueue = Engine::getInstance()->getGraphicsQueue();
        graphicsQueue->submit(1, &submitInfo, VK_NULL_HANDLE);
        graphicsQueue->waitIdle();

        commandBuffer.release(m_commandPool);
    }
}
