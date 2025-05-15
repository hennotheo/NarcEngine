#include "core/queues/PresentQueue.h"

#include "core/EngineBuilder.h"
#include "core/queues/QueueFamilyIndices.h"
#include "core/devices/LogicalDevice.h"
#include "core/devices/PhysicalDevice.h"

namespace narc_engine
{
    PresentQueue::PresentQueue(const EngineBuilder* builder)
    {
        QueueFamilyIndices indices = builder->getPhysicalDevice()->getQueueFamilyIndices();
        vkGetDeviceQueue(builder->getLogicalDevice()->get(), indices.PresentFamily.value(), 0, &m_queue);
    }

    VkResult PresentQueue::presentKHR(const VkPresentInfoKHR* presentInfo) const
    {
        return vkQueuePresentKHR(m_queue, presentInfo);
    }
} // namespace narc_engine
