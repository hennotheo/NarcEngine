#pragma once

#include <vulkan/vulkan.h>

#include "core/DeviceComponent.h"

namespace narc_engine
{
    class Fence : public DeviceComponent
    {
    public:
        Fence();
        ~Fence();

        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;

        GETTER VkFence getVkFence() const { return m_fence; }

    private:
        VkFence m_fence;
    };
} // namespace narc_engine
