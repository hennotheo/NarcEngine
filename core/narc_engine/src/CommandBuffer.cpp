#include "include/CommandBuffer.h"

#include "include/Pipeline.h"

namespace narc_engine {
    void CommandBuffer::allocate(const DeviceHandler* deviceHandler, const VkCommandBufferAllocateInfo* allocInfo)
    {
        if (vkAllocateCommandBuffers(deviceHandler->getDevice(), allocInfo, &m_commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void CommandBuffer::reset(VkCommandBufferResetFlags flags) const
    {
        if (vkResetCommandBuffer(m_commandBuffer, flags) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to reset command buffer!");
        }
    }

    void CommandBuffer::begin(VkCommandBufferBeginInfo beginInfo) const
    {
        if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }
    }

    VkResult CommandBuffer::end() const
    {
        return vkEndCommandBuffer(m_commandBuffer);
    }

    void CommandBuffer::release(const DeviceHandler* deviceHandler, const VkCommandPool& commandPool) const
    {
        vkFreeCommandBuffers(deviceHandler->getDevice(), commandPool, 1, &m_commandBuffer);
    }

    void CommandBuffer::cmdBeginRenderPass(const VkRenderPassBeginInfo* renderPassInfo, VkSubpassContents contents) const
    {
        vkCmdBeginRenderPass(m_commandBuffer, renderPassInfo, contents);
    }

    void CommandBuffer::cmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, const Pipeline* pipeline) const
    {
        vkCmdBindPipeline(m_commandBuffer, pipelineBindPoint, pipeline->m_pipeline);
    }

    void CommandBuffer::cmdSetViewport(const VkViewport* viewport, uint32_t firstViewport, uint32_t viewportCount) const
    {
        vkCmdSetViewport(m_commandBuffer, firstViewport, viewportCount, viewport);
    }

    void CommandBuffer::cmdSetScissor(const VkRect2D* scissor, uint32_t firstScissor, uint32_t scissorCount) const
    {
        vkCmdSetScissor(m_commandBuffer, firstScissor, scissorCount, scissor);
    }

    void CommandBuffer::cmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* buffers, const VkDeviceSize* offsets) const
    {
        vkCmdBindVertexBuffers(m_commandBuffer, firstBinding, bindingCount, buffers, offsets);
    }

    void CommandBuffer::cmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const
    {
        vkCmdBindIndexBuffer(m_commandBuffer, buffer, offset, indexType);
    }

    void CommandBuffer::cmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, const Pipeline* pipeline, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* descriptorSets,
                                              uint32_t dynamicOffsetCount, const uint32_t* dynamicOffsets) const
    {
        vkCmdBindDescriptorSets(m_commandBuffer, pipelineBindPoint, pipeline->m_pipelineLayout, firstSet, descriptorSetCount, descriptorSets, dynamicOffsetCount, dynamicOffsets);
    }

    void CommandBuffer::cmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const
    {
        vkCmdDrawIndexed(m_commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void CommandBuffer::cmdEndRenderPass() const
    {
        vkCmdEndRenderPass(m_commandBuffer);
    }

    void CommandBuffer::cmdPipelineBarrier(VkPipelineStageFlags sourceStage, VkPipelineStageFlags destinationStage, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount,
                                           const VkMemoryBarrier* memoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* bufferMemoryBarriers,
                                           uint32_t imageMemoryBarrierCount,
                                           const VkImageMemoryBarrier* imageMemoryBarriers) const
    {
        vkCmdPipelineBarrier(
            m_commandBuffer,
            sourceStage, destinationStage, dependencyFlags,
            memoryBarrierCount, memoryBarriers, bufferMemoryBarrierCount, bufferMemoryBarriers,
            imageMemoryBarrierCount, imageMemoryBarriers
        );
    }

    void CommandBuffer::cmdCopyBufferImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* regions) const
    {
        vkCmdCopyBufferToImage(m_commandBuffer,
                               srcBuffer, dstImage, dstImageLayout, regionCount, regions
        );
    }

    void CommandBuffer::cmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* copyRegion) const
    {
        vkCmdCopyBuffer(m_commandBuffer, srcBuffer, dstBuffer, regionCount, copyRegion);
    }

    void CommandBuffer::allocateBuffers(const DeviceHandler* deviceHandler, const VkCommandBufferAllocateInfo* allocInfo, std::vector<CommandBuffer>& commandBuffers)
    {
        for (auto& commandBuffer: commandBuffers)
        {
            if (commandBuffer.m_allocated)
            {
                throw std::runtime_error("Trying to allocate an already allocated command buffer!");
            }
        }

        std::vector<VkCommandBuffer> vkCommandBuffers(commandBuffers.size());
        if (vkAllocateCommandBuffers(deviceHandler->getDevice(), allocInfo, vkCommandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers.size(); ++i)
        {
            commandBuffers[i].m_commandBuffer = vkCommandBuffers[i];
            commandBuffers[i].m_allocated = true;
        }
    }
}
