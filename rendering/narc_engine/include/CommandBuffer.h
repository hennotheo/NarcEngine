#pragma once
#include "buffers/Buffer.h"

namespace narc_engine {
    class CommandBuffer
    {
    public:
        GETTER VkCommandBuffer getVkCommandBuffer() const { return m_commandBuffer; }

        void allocate(const DeviceHandler* deviceHandler, const VkCommandBufferAllocateInfo* allocInfo);
        void reset(VkCommandBufferResetFlags flags);
        void begin(VkCommandBufferBeginInfo beginInfo);
        VkResult end();
        void release(const DeviceHandler* deviceHandler, const VkCommandPool& commandPool);

        void cmdBeginRenderPass(const VkRenderPassBeginInfo* renderPassInfo, VkSubpassContents contents) const;
        void cmdBindPipeline(VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const;
        void cmdSetViewport(const VkViewport* viewport, uint32_t firstViewport, uint32_t viewportCount) const;
        void cmdSetScissor(const VkRect2D* scissor, uint32_t firstScissor, uint32_t scissorCount) const;
        void cmdBindVertexBuffers(uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* buffers, const VkDeviceSize* offsets) const;
        void cmdBindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const;
        void cmdBindDescriptorSets(VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const
                                   VkDescriptorSet* descriptorSets, uint32_t dynamicOffsetCount, const uint32_t* dynamicOffsets) const;
        void cmdDrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const;
        void cmdEndRenderPass() const;

        void cmdPipelineBarrier(VkPipelineStageFlags sourceStage, VkPipelineStageFlags destinationStage, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount,
                                const VkMemoryBarrier* memoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* bufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                                const VkImageMemoryBarrier* imageMemoryBarriers) const;
        void cmdCopyBufferImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* regions) const;
        void cmdCopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* copyRegion) const;

        static void allocateBuffers(const DeviceHandler* deviceHandler, const VkCommandBufferAllocateInfo* allocInfo, std::vector<CommandBuffer>& commandBuffers);

    private:
        VkCommandBuffer m_commandBuffer;
        bool m_allocated = false;
    };
}
