//
// Created by theo on 2/15/26.
//

#pragma once

namespace narc_engine::mapping {
    struct VulkanPresentInfo
    {
        std::vector<VkSwapchainKHR> SwapChains;
        std::vector<VkSemaphore> WaitSemaphores;
        VkPresentInfoKHR Infos{};
    };

    struct VulkanSubmitInfo
    {
        std::vector<VkCommandBuffer> CommandBuffers;
        std::vector<VkSemaphore> SignalSemaphores;
        std::vector<VkSemaphore> WaitSemaphores;
        VkSubmitInfo Infos{};
        VkPipelineStageFlags WaitStageMask;
    };

    VkExtent2D mapFromExtend(const narc_math::Extent& extent) noexcept;

    VkApplicationInfo mapFromApplicationInfo(const ApplicationInfo& value) noexcept;

    VkPipelineStageFlags mapFromSubmitPipelineWaitStage(const SubmitWaitStageMask& value) noexcept;
    VkViewport mapFromViewPortInfos(const ViewPortInfos& value) noexcept;
    VkRect2D mapFromScissorsInfos(const ScissorsInfos& value) noexcept;

    VulkanSubmitInfo mapFromQueueSubmitInfos(const QueueSubmitInfos& value) noexcept;
    VulkanPresentInfo mapFromQueuePresentInfos(const QueuePresentInfos& value) noexcept;
    VkFormat mapFromVertexAttributeFormat(const VertexAttributeFormat& value) noexcept;
    VkVertexInputAttributeDescription mapFromVertexAttribute(const VertexAttribute& value) noexcept;
    VkVertexInputBindingDescription mapFromVertexLayout(const VertexLayout& value) noexcept;
    VkImageLayout mapFromImageLayout(const ImageLayout& value) noexcept;

    std::vector<VkFence_T*> toVkFenceArray(std::span<const IFence*> fences) noexcept;
}
