//
// Created by theohenno on 11/30/25.
//

#include "command/VulkanCommandBuffer.h"

#include <X11/Xlib.h>

#include "buffers/interfaces/IVulkanBuffer.h"
#include "descriptor/VulkanDescriptorSet.h"
#include "pipeline/VulkanGraphicsPipeline.h"
#include "pipeline/VulkanPipelineLayout.h"
#include "pipeline/VulkanRenderPass.h"
#include "buffers/VulkanVertexBuffer.h"
#include "buffers/VulkanIndexBuffer.h"
#include "mapping/mappingToVk.h"
#include "swapchain/VulkanSwapChain.h"

namespace narc_engine {
    VulkanCommandBuffer::VulkanCommandBuffer(const VkCommandBuffer commandBuffer) :
        m_commandBuffer(commandBuffer)
    {
        //Empty constructor.
    }

    VulkanCommandBuffer::~VulkanCommandBuffer() noexcept = default;

    void VulkanCommandBuffer::cmdPipelineBarrier(const VkPipelineStageFlags& srcStage,
                                                 const VkPipelineStageFlags& dstStage,
                                                 const uint32_t imageBarrierCount,
                                                 const VkImageMemoryBarrier& barrier) const
    {
        vkCmdPipelineBarrier(m_commandBuffer,
                             srcStage,
                             dstStage,
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             imageBarrierCount,
                             &barrier);
    }

    void VulkanCommandBuffer::cmdBindVertexBuffers(VulkanVertexBuffer& vertexBuffer)
    {
        const std::array vertexBuffers = {vertexBuffer.getHandle()};
        const std::vector<VkDeviceSize> offsets = {0};
        vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, vertexBuffers.data(), offsets.data());
    }

    void VulkanCommandBuffer::cmdBindIndexBuffers(VulkanIndexBuffer& indexBuffer)
    {
        const std::vector<VkDeviceSize> offsets = {0};
        vkCmdBindIndexBuffer(m_commandBuffer, indexBuffer.getHandle(), 0, VK_INDEX_TYPE_UINT16);
    }

    void VulkanCommandBuffer::cmdBindDescriptorSets(std::span<VulkanDescriptorSet> descriptorSets, VulkanPipelineLayout& pipelineLayout)
    {
        std::vector<VkDescriptorSet> layouts;
        layouts.reserve(descriptorSets.size());
        std::ranges::transform(descriptorSets, std::back_inserter(layouts),
                               [](const VulkanDescriptorSet& descriptorSet) {
                                   return descriptorSet.getHandle();
                               });

        vkCmdBindDescriptorSets(m_commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout.getHandle(),
                                0,
                                descriptorSets.size(),
                                layouts.data(),
                                0,
                                nullptr);
    }

    void VulkanCommandBuffer::cmdSetViewport(const VkViewport& viewport)
    {
        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
    }

    void VulkanCommandBuffer::cmdSetScissor(const VkRect2D& scissor)
    {
        vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
    }

    void VulkanCommandBuffer::cmdDraw()
    {
        vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);
    }

    void VulkanCommandBuffer::cmdDrawIndexed(const uint32_t indexCount)
    {
        vkCmdDrawIndexed(m_commandBuffer, indexCount, 1, 0, 0, 0);
    }

    void VulkanCommandBuffer::cmdCopyBuffer(const VkBufferCopy& infos, const IVulkanBuffer& src, const IVulkanBuffer& dst)
    {
        vkCmdCopyBuffer(m_commandBuffer, src.getHandle(), dst.getHandle(), 1, &infos);
    }

    void VulkanCommandBuffer::cmdCopyBufferToImage(const IVulkanBuffer& src, const VkImage& dst, const VkBufferImageCopy& infos)
    //TODO: Edit vkimg to img
    {
        vkCmdCopyBufferToImage(m_commandBuffer, src.getHandle(), dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &infos);
    }

    narc_core::result VulkanCommandBuffer::begin() const noexcept
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = Flags;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            return false;
        }

        return true;
    }

    narc_core::result VulkanCommandBuffer::end() const noexcept
    {
        return vkEndCommandBuffer(m_commandBuffer) == VK_SUCCESS;
    }

    narc_core::result VulkanCommandBuffer::reset() const noexcept
    {
        return vkResetCommandBuffer(m_commandBuffer, 0) == VK_SUCCESS;
    }

    narc_core::result VulkanCommandBuffer::copyBuffer(const IBuffer* source, const IBuffer* destination) const noexcept
    {
        return false;
    }

    narc_core::result VulkanCommandBuffer::beginRenderPass(const ISwapchain* swapChain, const RenderPassInfos& infos) const noexcept
    {
        auto extent = swapChain->getSwapChainExtent();
        auto vkSwapChain = backend_cast<VulkanSwapChain>(swapChain);
        auto vkPipeline = backend_cast<VulkanGraphicsPipeline>(infos.TEMPPipeline);
        NARC_LOG_WARNING("Temporary bad Architecture for begining renderpass.");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkPipeline->getRenderPass()->getHandle();
        renderPassInfo.framebuffer = vkSwapChain->getFrameBuffer(infos.TEMPFrameInFlightIndex)->getHandle();
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {extent.Width, extent.Height};

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1; //TODO: HARDCODED
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        return true;
    }

    narc_core::result VulkanCommandBuffer::endRenderPass() const noexcept
    {
        vkCmdEndRenderPass(m_commandBuffer);

        return true;
    }

    narc_core::result VulkanCommandBuffer::bindPipeline(const IGraphicsPipeline* pipeline) const noexcept
    {
        const auto& vkPipeline = backend_cast<VulkanGraphicsPipeline>(pipeline);

        vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline->getHandle());

        return true;
    }

    narc_core::result VulkanCommandBuffer::bindViewPort(const ViewPortInfos& viewport) const noexcept
    {
        const auto vkViewport = mapping::mapFromViewPortInfos(viewport);

        vkCmdSetViewport(m_commandBuffer, 0, 1, &vkViewport);

        return true;
    }

    narc_core::result VulkanCommandBuffer::draw() const noexcept
    {
        vkCmdDraw(m_commandBuffer, 0, 0, 0, 0);

        return true;
    }

    narc_core::result VulkanCommandBuffer::bindScissors(const ScissorsInfos& scissors) const noexcept
    {
        const auto vkScissors = mapping::mapFromScissorsInfos(scissors);

        vkCmdSetScissor(m_commandBuffer, 0, 1, &vkScissors);

        return true;
    }
} // narc_engine
