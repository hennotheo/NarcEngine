//
// Created by theohenno on 11/30/25.
//

#include "command/VulkanCommandBuffer.h"

#include "VulkanTextureImage.h"
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

    narc_core::result VulkanCommandBuffer::copyBuffer(const IBuffer* source, const IBuffer* destination, const MemorySize size) const noexcept
    {
        const auto vkSrc = narc_core::backend_cast<IVulkanBuffer>(source);
        const auto vkDst = narc_core::backend_cast<IVulkanBuffer>(destination);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;

        vkCmdCopyBuffer(m_commandBuffer, vkSrc->getHandle(), vkDst->getHandle(), 1, &copyRegion);

        return false;
    }

    narc_core::result VulkanCommandBuffer::beginRenderPass(const ISwapchain* swapChain, const RenderPassInfos& infos) const noexcept
    {
        auto extent = swapChain->getSwapChainExtent();
        auto vkSwapChain = narc_core::backend_cast<VulkanSwapChain>(swapChain);
        auto vkPipeline = narc_core::backend_cast<VulkanGraphicsPipeline>(infos.TEMPPipeline);
        //TODO: NARC_LOG_WARNING("Temporary bad Architecture for begining renderpass.");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkPipeline->getRenderPass()->getHandle();
        renderPassInfo.framebuffer = vkSwapChain->getFrameBuffer(infos.TEMPImageIndex)->getHandle();
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
        vkCmdDraw(m_commandBuffer, 4, 1, 0, 0);

        return true;
    }

    narc_core::result VulkanCommandBuffer::drawIndexed(const uint32_t indexCount) const noexcept
    {
        vkCmdDrawIndexed(m_commandBuffer, indexCount, 1, 0, 0, 0);

        return true;
    }

    narc_core::result VulkanCommandBuffer::bindScissors(const ScissorsInfos& scissors) const noexcept
    {
        const auto vkScissors = mapping::mapFromScissorsInfos(scissors);

        vkCmdSetScissor(m_commandBuffer, 0, 1, &vkScissors);

        return true;
    }

    narc_core::result VulkanCommandBuffer::bindIndexBuffer(const IBuffer* buffer) const noexcept
    {
        const auto vkBuffer = narc_core::backend_cast<IVulkanBuffer>(buffer);

        vkCmdBindIndexBuffer(m_commandBuffer, vkBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT16);

        return true;
    }

    narc_core::result VulkanCommandBuffer::bindDescriptorSets(const IPipelineLayout* layout, const IDescriptorBinding* binding) const noexcept
    {
        // std::vector<VkDescriptorSet> layouts;
        // layouts.reserve(descriptorSets.size());
        // std::ranges::transform(descriptorSets, std::back_inserter(layouts),
        //                        [](const VulkanDescriptorSet& descriptorSet) {
        //                            return descriptorSet.getHandle();
        //                        });
        const auto set = narc_core::backend_cast<VulkanDescriptorSet>(binding);
        const auto vkPipelineLayout = narc_core::backend_cast<VulkanPipelineLayout>(layout);

        const std::array sets{set->getHandle()};
        vkCmdBindDescriptorSets(m_commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                vkPipelineLayout->getHandle(),
                                0,
                                sets.size(),
                                sets.data(),
                                0,
                                nullptr);
        return true;
    }

    narc_core::result VulkanCommandBuffer::transitionImageLayout(const IImage* image, const ImageLayout oldLayout, const ImageLayout newLayout) const noexcept
    {
        const auto vkImage = narc_core::backend_cast<VulkanTextureImage>(image);

        const auto vkOldLayout = mapping::mapFromImageLayout(oldLayout);
        const auto vkNewLayout = mapping::mapFromImageLayout(newLayout);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = vkOldLayout;
        barrier.newLayout = vkNewLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = vkImage->getImage();
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (vkOldLayout == VK_IMAGE_LAYOUT_UNDEFINED && vkNewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (vkOldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && vkNewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
                m_commandBuffer,
                sourceStage,
                destinationStage,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1, &barrier
                );

        return true;
    }

    narc_core::result VulkanCommandBuffer::copyBufferToImage(const IBuffer* source, const IImage* destination) const noexcept
    {
        const auto vkImg = narc_core::backend_cast<VulkanTextureImage>(destination);
        const auto vkBuffer = narc_core::backend_cast<IVulkanBuffer>(source);

        const auto extent = vkImg->getExtent();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
            extent.Width,
            extent.Height,
            1
        };

        vkCmdCopyBufferToImage(m_commandBuffer, vkBuffer->getHandle(), vkImg->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        return true;
    }

    narc_core::result VulkanCommandBuffer::bindVertexBuffers(const IBuffer* buffer) const noexcept
    {
        //TODO: Multiple buffer binding support
        auto vkBuffer = narc_core::backend_cast<IVulkanBuffer>(buffer);
        std::array vertexBuffers = {vkBuffer->getHandle()};
        const std::vector<VkDeviceSize> offsets = {0};

        vkCmdBindVertexBuffers(m_commandBuffer, 0, vertexBuffers.size(), vertexBuffers.data(), offsets.data());

        return true;
    }
} // narc_engine
