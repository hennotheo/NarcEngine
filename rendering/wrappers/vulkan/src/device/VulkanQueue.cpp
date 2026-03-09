//
// Created by theo on 10/30/25.
//

#include "device/VulkanQueue.h"

#include "device/VulkanDevice.h"
#include "sync/VulkanFence.h"
#include "helpers/QueueHelpers.h"
#include "mapping/mappingToVk.h"

namespace narc_engine {
    VulkanQueue::VulkanQueue() :
        m_queueFamilyIndex(QUEUE_INDEX_NONE),
        m_queueIndex(QUEUE_INDEX_NONE)
    {

    }

    VulkanQueue::~VulkanQueue() = default;

    void VulkanQueue::init()
    {
        if (!checkIndicesDefined())
        {
            return;
        }

        NARC_GUARD_RAW_PTR(m_device, "Device not defined.");

        vkGetDeviceQueue(m_device->getHandle(), m_queueFamilyIndex, m_queueIndex, &m_queue);
        NARC_LOG_DEBUG("Vulkan Queue created successfully!");
    }

    void VulkanQueue::shutdown() { m_queue = VK_NULL_HANDLE; }

    VkResult VulkanQueue::submit(const uint32_t submitCount, const VkSubmitInfo& infos, const VulkanFence* fence) const
    {
        VkFence pfence = fence == nullptr ? VK_NULL_HANDLE : fence->getHandle();

        return vkQueueSubmit(m_queue, submitCount, &infos, pfence);
    }

    void VulkanQueue::waitIdle() const
    {
        vkQueueWaitIdle(m_queue);
    }

    narc_core::result VulkanQueue::submit(const QueueSubmitInfos infos) const noexcept
    {
        const auto submitInfos = mapping::mapFromQueueSubmitInfos(infos);
        const VkFence vkFence = infos.Fence == nullptr ? VK_NULL_HANDLE : dynamic_cast<const VulkanFence*>(infos.Fence)->getHandle();

        if (vkQueueSubmit(m_queue, 1, &submitInfos.Infos, vkFence) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    QueuePresentResult VulkanQueue::present(const QueuePresentInfos infos) const noexcept
    {
        const auto submitInfos = mapping::mapFromQueuePresentInfos(infos);

        const auto result = vkQueuePresentKHR(m_queue, &submitInfos.Infos);
        if (result != VK_SUCCESS)
        {
            return QueuePresentResult{
                    .HasError = true,
                    .IsOutOfDate = result == VK_ERROR_OUT_OF_DATE_KHR,
                    .IsSuboptimal = result == VK_SUBOPTIMAL_KHR,
            };
        }

        return QueuePresentResult{};
    }

    narc_core::result VulkanQueue::waitQueueIdle() const noexcept
    {
        return vkQueueWaitIdle(m_queue) == VK_SUCCESS;
    }

    bool VulkanQueue::isIndexDefined(const uint32_t& index)
    {
        return QUEUE_INDEX_NONE != index;
    }

    bool VulkanQueue::checkIndicesDefined()
    {
        bool defined = true;

        if (!isIndexDefined(m_queueIndex))
        {
            NARC_LOG_WARNING("Queue Index is not defined.");
            defined = false;
        }

        if (!isIndexDefined(m_queueFamilyIndex))
        {
            NARC_LOG_WARNING("Queue Family Index is not defined.");
            defined = false;
        }

        return defined;
    }
} // namespace narc_engine
