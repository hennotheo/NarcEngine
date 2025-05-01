#include "core/queues/GraphicsQueue.h"

#include "core/EngineBuilder.h"
#include "core/queues/QueueFamilyIndices.h"
#include "core/devices/LogicalDevice.h"
#include "core/devices/PhysicalDevice.h"

namespace narc_engine
{
    GraphicsQueue::GraphicsQueue(const EngineBuilder* builder)
    {
        QueueFamilyIndices indices = builder->getPhysicalDevice()->getQueueFamilyIndices();
        vkGetDeviceQueue(builder->getLogicalDevice()->getVkDevice(), indices.GraphicsFamily.value(), 0, &m_queue);
    }

    void GraphicsQueue::waitIdle() const
    {
        if(vkQueueWaitIdle(m_queue) != VK_SUCCESS)
        {
            NARCLOG_FATAL("Failed to wait for graphics queue idle!");
        }
    }

    VkResult GraphicsQueue::submit(uint32_t submitCount, const VkSubmitInfo* submitInfo, VkFence fence) const
    {
        return vkQueueSubmit(m_queue, submitCount, submitInfo, fence);
    }
} // namespace narc_engine