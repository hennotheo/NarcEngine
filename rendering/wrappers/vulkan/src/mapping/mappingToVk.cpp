//
// Created by theo on 2/15/26.
//

#include "mapping/mappingToVk.h"

#include "command/VulkanCommandBuffer.h"
#include "swapchain/VulkanSwapChain.h"
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

    VkPipelineStageFlags mapFromSubmitPipelineWaitStage(const SubmitWaitStageMask& stages) noexcept
    {
        VkPipelineStageFlags flags = 0;

        const auto value = static_cast<uint32_t>(stages);

        if (value & static_cast<uint32_t>(SubmitWaitStageMask::ColorAttachmentOutput))
        {
            flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }

        if (value & static_cast<uint32_t>(SubmitWaitStageMask::VertexShader))
        {
            flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
        }

        if (value & static_cast<uint32_t>(SubmitWaitStageMask::FragmentShader))
        {
            flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        if (value & static_cast<uint32_t>(SubmitWaitStageMask::ComputeShader))
        {
            flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        }

        return flags;
    }

    VkViewport mapFromViewPortInfos(const ViewPortInfos& value) noexcept
    {
        return VkViewport{
                .x = value.Position.X(),
                .y = value.Position.Y(),
                .width = static_cast<float>(value.Dimensions.Width),
                .height = static_cast<float>(value.Dimensions.Height),
                .minDepth = 0.0f,
                .maxDepth = 1.0f
        };
    }

    VkRect2D mapFromScissorsInfos(const ScissorsInfos& value) noexcept
    {
        return {
                .offset = {
                        .x = value.Offset.X(),
                        .y = value.Offset.Y()
                },
                .extent = {
                        .width = value.Extent.Width,
                        .height = value.Extent.Height
                }
        };
    }

    VulkanSubmitInfo mapFromQueueSubmitInfos(const QueueSubmitInfos& value) noexcept
    {
        VulkanSubmitInfo info{};
        info.CommandBuffers = value.CommandBuffers
                              | narc_core::transform_to_concrete_class<VulkanCommandBuffer>()
                              | std::views::transform([](auto* cmdBuffer) noexcept {
                                  return cmdBuffer->getHandle();
                              })
                              | narc_core::to<std::vector>();
        info.WaitSemaphores = value.WaitSemaphores
                              | narc_core::transform_to_concrete_class<VulkanSemaphore>()
                              | std::views::transform([](auto* cmdBuffer) noexcept {
                                  return cmdBuffer->getHandle();
                              })
                              | narc_core::to<std::vector>();
        info.SignalSemaphores = value.SignalSemaphores
                                | narc_core::transform_to_concrete_class<VulkanSemaphore>()
                                | std::views::transform([](auto* cmdBuffer) noexcept {
                                    return cmdBuffer->getHandle();
                                })
                                | narc_core::to<std::vector>();
        info.WaitStageMask = mapFromSubmitPipelineWaitStage(value.WaitStages);

        info.Infos = VkSubmitInfo{
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext = nullptr,
                .waitSemaphoreCount = static_cast<uint32_t>(info.WaitSemaphores.size()),
                .pWaitSemaphores = info.WaitSemaphores.data(),
                .pWaitDstStageMask = &info.WaitStageMask,
                .commandBufferCount = static_cast<uint32_t>(info.CommandBuffers.size()),
                .pCommandBuffers = info.CommandBuffers.data(),
                .signalSemaphoreCount = static_cast<uint32_t>(info.SignalSemaphores.size()),
                .pSignalSemaphores = info.SignalSemaphores.data()
        };

        return info;
    }

    VulkanPresentInfo mapFromQueuePresentInfos(const QueuePresentInfos& value) noexcept
    {
        VulkanPresentInfo infos{};
        infos.SwapChains = value.SwapChains
                           | narc_core::transform_to_concrete_class<VulkanSwapChain>()
                           | std::views::transform([](auto* swapchain) noexcept {
                               return swapchain->getHandle();
                           })
                           | narc_core::to<std::vector>();
        infos.WaitSemaphores = value.WaitSemaphores
                               | narc_core::transform_to_concrete_class<VulkanSemaphore>()
                               | std::views::transform([](auto* cmdBuffer) noexcept {
                                   return cmdBuffer->getHandle();
                               })
                               | narc_core::to<std::vector>();

        infos.Infos = VkPresentInfoKHR{
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .pNext = nullptr,
                .waitSemaphoreCount = static_cast<uint32_t>(infos.WaitSemaphores.size()),
                .pWaitSemaphores = infos.WaitSemaphores.data(),
                .swapchainCount = static_cast<uint32_t>(infos.SwapChains.size()),
                .pSwapchains = infos.SwapChains.data(),
                .pImageIndices = value.ImageIndices.data(),
                .pResults = nullptr
        };

        return infos;
    }

    VkFormat mapFromVertexAttributeFormat(const VertexAttributeFormat& value) noexcept
    {
        switch (value)
        {
            case VertexAttributeFormat::Float: return VK_FORMAT_R32_SFLOAT;
            case VertexAttributeFormat::Float2: return VK_FORMAT_R32G32_SFLOAT;
            case VertexAttributeFormat::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
            case VertexAttributeFormat::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
        }

        NARC_LOG_WARNING("VertexAttributeFormat is not implemented in Vulkan.");
        return VK_FORMAT_UNDEFINED;
    }

    VkVertexInputAttributeDescription mapFromVertexAttribute(const VertexAttribute& value) noexcept
    {
        VkVertexInputAttributeDescription description{};
        description.binding = value.Binding;
        description.location = value.Location;
        description.format = mapFromVertexAttributeFormat(value.Format);
        description.offset = value.Offset;

        return description;
    }

    VkVertexInputBindingDescription mapFromVertexLayout(const VertexLayout& value) noexcept
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0; //TEMP
        bindingDescription.stride = value.Stride;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //TEMP

        return bindingDescription;
    }

    VkImageLayout mapFromImageLayout(const ImageLayout& value) noexcept
    {
        switch (value)
        {
            case Undefined: return VK_IMAGE_LAYOUT_UNDEFINED;
            case TransferSource: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case TransferDestination: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case ShaderReadOnly: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        NARC_LOG_WARNING("This Image Layout is not implemented in Vulkan.");
        return VK_IMAGE_LAYOUT_UNDEFINED;
    }

    std::vector<VkFence_T*> toVkFenceArray(std::span<const IFence*> fences) noexcept
    {
        return fences
               | narc_core::transform_to_concrete_class<const VulkanFence>()
               | std::views::transform([](const auto* f) { return f->getHandle(); })
               | narc_core::to<std::vector>();

    }
}

