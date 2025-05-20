#pragma once

#include <vulkan/vulkan.h>

namespace narc_engine
{
    class Semaphore : public narc_core::IGetter<VkSemaphore>
    {
    public:
        Semaphore();
        ~Semaphore();

        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        NARC_IMPL_IGETTER(VkSemaphore, m_semaphore)

    private:
        VkSemaphore m_semaphore;
    };
} // namespace narc_engine
