#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"

namespace narc_engine
{
    class Fence : public DeviceComponent, public narc_core::IGetter<VkFence>
    {
    public:
        Fence();
        ~Fence();
        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;

        NARC_IMPL_IGETTER(VkFence, m_fence)

    private:
        VkFence m_fence;
    };
} // namespace narc_engine