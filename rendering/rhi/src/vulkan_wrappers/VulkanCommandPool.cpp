//
// Created by theohenno on 11/30/25.
//

#include "vulkan_wrappers/VulkanCommandPool.h"

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
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void VulkanCommandPool::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        
        vkDestroyCommandPool(device->getHandle(), m_commandPool, nullptr);
    }
} // narc_engine
