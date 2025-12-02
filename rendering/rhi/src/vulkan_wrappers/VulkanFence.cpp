//
// Created by theohenno on 12/1/25.
//

#include "vulkan_wrappers/VulkanFence.h"

#include "vulkan_wrappers/VulkanDevice.h"

namespace narc_engine {
    VulkanFence::VulkanFence(std::weak_ptr<VulkanDevice> device) :
        m_device(std::move(device))
    {
    }

    VulkanFence::~VulkanFence() = default;

    void VulkanFence::init()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        if (vkCreateFence(device->getHandle(), &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create semaphores!");
        }
    }

    void VulkanFence::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        vkDestroyFence(device->getHandle(), m_fence, nullptr);
    }

    void VulkanFence::wait(const uint64_t timeout) const
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        vkWaitForFences(device->getHandle(), 1, &m_fence, VK_TRUE, timeout);
    }

    void VulkanFence::reset() const
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        vkResetFences(device->getHandle(), 1, &m_fence);
    }
} // narc_engine
