#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"

namespace narc_engine
{
    class Semaphore : public DeviceComponent
    {
    public:
        Semaphore();
        ~Semaphore();

        operator VkSemaphore() const { return m_semaphore; }

        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        GETTER VkSemaphore getVkSemaphore() const { return m_semaphore; }

    private:
        VkSemaphore m_semaphore;
    };
} // namespace narc_engine
