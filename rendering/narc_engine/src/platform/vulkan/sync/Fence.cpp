#include "platform/vulkan/sync/Fence.h"

namespace narc_engine
{
    Fence::Fence() : DeviceComponent()
    {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(getVkDevice(), &fenceInfo, nullptr, &m_fence) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to create fence!");
        }
    }

    Fence::~Fence()
    {
        vkDestroyFence(getVkDevice(), m_fence, nullptr);
    }
} // namespace narc_engine
