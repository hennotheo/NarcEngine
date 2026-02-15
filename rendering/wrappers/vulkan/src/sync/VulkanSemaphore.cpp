//
// Created by theohenno on 12/1/25.
//

#include "sync/VulkanSemaphore.h"

#include "device/VulkanDevice.h"

namespace narc_engine {
    VulkanSemaphore::VulkanSemaphore(std::weak_ptr<VulkanDevice> device) :
        m_device(std::move(device))
    {
    }

    VulkanSemaphore::~VulkanSemaphore() = default;

    void VulkanSemaphore::init()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        if (vkCreateSemaphore(device->getHandle(), &semaphoreInfo, nullptr, &m_semaphore) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create semaphore!");
        }
    }

    void VulkanSemaphore::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        
        vkDestroySemaphore(device->getHandle(), m_semaphore, nullptr);
    }
} // narc_engine
