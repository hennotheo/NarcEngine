//
// Created by theohenno on 12/1/25.
//

#include "sync/VulkanFence.h"

#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanFence::VulkanFence(const VulkanDevice* device) :
        m_device(device)
    {
    }

    VulkanFence::~VulkanFence() = default;

    void VulkanFence::init()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        if (vkCreateFence(m_device->getHandle(), &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create semaphores!");
        }
    }

    void VulkanFence::shutdown()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");
        vkDestroyFence(m_device->getHandle(), m_fence, nullptr);
    }

    void VulkanFence::wait(const uint64_t timeout) const
    {
        vkWaitForFences(m_device->getHandle(), 1, &m_fence, VK_TRUE, timeout);
    }

    void VulkanFence::reset() const
    {
        vkResetFences(m_device->getHandle(), 1, &m_fence);
    }
} // narc_engine
