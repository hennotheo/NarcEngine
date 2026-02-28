//
// Created by theohenno on 11/30/25.
//

#include "command/VulkanCommandPool.h"

#include "command/VulkanCommandBuffer.h"
#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanCommandPool::VulkanCommandPool(const VulkanDevice* device) :
        m_device(device)
    {
    }

    VulkanCommandPool::~VulkanCommandPool() noexcept = default;

    void VulkanCommandPool::init()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");

        const auto queueFamilyIndices = m_device->getQueueFamilyIndices();

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(m_device->getHandle(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create command pool!");
        }
    }

    void VulkanCommandPool::shutdown()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");
        
        vkDestroyCommandPool(m_device->getHandle(), m_commandPool, nullptr);
    }

    RhiQuery<std::unique_ptr<ICommandBuffer>> VulkanCommandPool::allocateOneTimeBuffer() const noexcept
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(m_device->getHandle(), &allocInfo, &commandBuffer) != VK_SUCCESS)
        {
            return RhiUnexpected("Failed to allocate command buffers!");
        }
        
        auto cmdBuffer = std::make_unique<VulkanCommandBuffer>(commandBuffer);
        cmdBuffer->Flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        
        return cmdBuffer;
    }

    RhiQuery<std::unique_ptr<ICommandBuffer>> VulkanCommandPool::allocateCommandBuffer() const noexcept
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(m_device->getHandle(), &allocInfo, &commandBuffer) != VK_SUCCESS)
        {
            return RhiUnexpected("Failed to allocate command buffers!");
        }
        
        return std::make_unique<VulkanCommandBuffer>(commandBuffer);
    }

    void VulkanCommandPool::freeBuffer(const VulkanCommandBuffer& buffers)
    {
        const std::array commandBuffers{buffers.getHandle()};
        vkFreeCommandBuffers(m_device->getHandle(), m_commandPool,1, commandBuffers.data());
    }
} // narc_engine
