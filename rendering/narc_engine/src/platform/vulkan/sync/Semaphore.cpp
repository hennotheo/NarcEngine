#include "platform/vulkan/sync/Semaphore.h"

namespace narc_engine
{
    Semaphore::Semaphore()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(NARC_DEVICE_HANDLE, &semaphoreInfo, nullptr, &m_semaphore) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create semaphore!");
        }
    }

    Semaphore::~Semaphore()
    {
        vkDestroySemaphore(NARC_DEVICE_HANDLE, m_semaphore, nullptr);
    }
} // namespace narc_engine