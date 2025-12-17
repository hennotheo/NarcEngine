//
// Created by theohenno on 11/30/25.
//

#include "vulkan_wrappers/VulkanCommandPool.h"

#include "vulkan_wrappers/VulkanCommandBuffer.h"
#include "vulkan_wrappers/VulkanDevice.h"

namespace narc_engine {
    VulkanCommandPool::VulkanCommandPool(std::weak_ptr<VulkanDevice> device) :
        m_device(std::move(device))
    {
    }

    VulkanCommandPool::~VulkanCommandPool() noexcept = default;

    void VulkanCommandPool::init()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        const auto queueFamilyIndices = device->getQueueFamilyIndices();

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

        if (vkCreateCommandPool(device->getHandle(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create command pool!");
        }
    }

    void VulkanCommandPool::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        
        vkDestroyCommandPool(device->getHandle(), m_commandPool, nullptr);
    }

    std::unique_ptr<VulkanCommandBuffer> VulkanCommandPool::allocateCopyBuffer() const
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(device->getHandle(), &allocInfo, &commandBuffer) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to allocate command buffers!");
        }
        
        auto cmdBuffer = std::make_unique<VulkanCommandBuffer>(commandBuffer);
        cmdBuffer->Flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        
        return std::move(cmdBuffer);
    }

    std::unique_ptr<VulkanCommandBuffer> VulkanCommandPool::allocateCommandBuffer() const
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        if (vkAllocateCommandBuffers(device->getHandle(), &allocInfo, &commandBuffer) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to allocate command buffers!");
        }
        
        return std::make_unique<VulkanCommandBuffer>(commandBuffer);
    }

    void VulkanCommandPool::freeBuffer(const VulkanCommandBuffer& buffers)
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        const std::array commandBuffers{buffers.getHandle()};
        vkFreeCommandBuffers(device->getHandle(), m_commandPool,1, commandBuffers.data());
    }
} // narc_engine
