//
// Created by theo on 2/15/26.
//

#include "mapping/mappingToVk.h"

#include "command/VulkanCommandBuffer.h"
#include "sync/VulkanFence.h"
#include "sync/VulkanSemaphore.h"

namespace narc_engine::mapping {
    VkExtent2D mapFromExtend(const narc_math::Extent& extent) noexcept
    {
        return {
                extent.Width,
                extent.Height
        };
    }

    VkApplicationInfo mapFromApplicationInfo(const ApplicationInfo& value) noexcept
    {
        return VkApplicationInfo{
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = value.ApplicationName.c_str(),
                .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
                .pEngineName = value.EngineName.c_str(),
                .engineVersion = VK_MAKE_VERSION(0, 1, 0),
                .apiVersion = VK_API_VERSION_1_3
        };
    }

    VkSubmitInfo mapFromQueueSubmitInfos(const QueueSubmitInfos& value) noexcept
    {
        const auto vulkanCommandBuffers = value.CommandBuffers
                                          | narc_core::transform_to_concrete_class<VulkanCommandBuffer>()
                                          | std::views::transform([](auto* cmdBuffer) noexcept {
                                              return cmdBuffer->getHandle();
                                          })
                                          | narc_core::to<std::vector>();
        const auto waitSemaphores = value.WaitSemaphores
                                    | narc_core::transform_to_concrete_class<VulkanSemaphore>()
                                    | std::views::transform([](auto* cmdBuffer) noexcept {
                                        return cmdBuffer->getHandle();
                                    })
                                    | narc_core::to<std::vector>();
        const auto signalSemaphores = value.SignalSemaphores
                                      | narc_core::transform_to_concrete_class<VulkanSemaphore>()
                                      | std::views::transform([](auto* cmdBuffer) noexcept {
                                          return cmdBuffer->getHandle();
                                      })
                                      | narc_core::to<std::vector>();

        return VkSubmitInfo{
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext = nullptr,
                .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()),
                .pWaitSemaphores = waitSemaphores.data(),
                .pWaitDstStageMask = nullptr,
                .commandBufferCount = static_cast<uint32_t>(vulkanCommandBuffers.size()),
                .pCommandBuffers = vulkanCommandBuffers.data(),
                .signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
                .pSignalSemaphores = signalSemaphores.data(),
        };
    }

    std::vector<VkFence_T*> toVkFenceArray(std::span<const IFence*> fences) noexcept
    {
        return fences
               | narc_core::transform_to_concrete_class<const VulkanFence>()
               | std::views::transform([](const auto* f) { return f->getHandle(); })
               | narc_core::to<std::vector>();

    }
}

