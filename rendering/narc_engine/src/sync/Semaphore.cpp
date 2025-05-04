#include "sync/Semaphore.h"

namespace narc_engine
{
    Semaphore::Semaphore() : DeviceComponent()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(getVkDevice(), &semaphoreInfo, nullptr, &m_semaphore) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create semaphore!");
        }
    }

    Semaphore::~Semaphore()
    {
        vkDestroySemaphore(getVkDevice(), m_semaphore, nullptr);
    }
} // namespace narc_engine
