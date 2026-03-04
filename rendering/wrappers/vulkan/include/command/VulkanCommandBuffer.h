//
// Created by theohenno on 11/30/25.
//

#pragma once

namespace narc_engine {
    class VulkanPipelineLayout;
    class VulkanDescriptorSet;
    class VulkanIndexBuffer;
    class VulkanVertexBuffer;
    class VulkanFramebuffer;
    class VulkanGraphicsPipeline;
    class VulkanRenderPass;
    class VulkanCommandPool;
    class VulkanDevice;
    class IVulkanBuffer;

    // TODO: TEMP TO BE EDITED LATER
    class IRenderPassCmdBuffer
    {
    protected:
        ~IRenderPassCmdBuffer() = default;

    public:
        virtual void cmdBindVertexBuffers(VulkanVertexBuffer& vertexBuffer) = 0;
        virtual void cmdBindIndexBuffers(VulkanIndexBuffer& indexBuffer) = 0;
        virtual void cmdBindDescriptorSets(std::span<VulkanDescriptorSet> descriptorSets, VulkanPipelineLayout& pipelineLayout) = 0;
        virtual void cmdSetViewport(const VkViewport& viewport) = 0;
        virtual void cmdSetScissor(const VkRect2D& scissor) = 0;
        virtual void cmdDraw() = 0;
        virtual void cmdDrawIndexed(uint32_t indexCount) = 0;
    };

    class VulkanCommandBuffer final : public ICommandBuffer, public IRenderPassCmdBuffer
    {
    public:
        VkCommandBufferUsageFlags Flags = 0;
        
    public:
        explicit VulkanCommandBuffer(VkCommandBuffer m_commandBuffer);
        ~VulkanCommandBuffer() noexcept override;
        
        void cmdPipelineBarrier(const VkPipelineStageFlags& srcStage, const VkPipelineStageFlags& dstStage, uint32_t imageBarrierCount, const VkImageMemoryBarrier& barrier) const;

        void cmdBindVertexBuffers(VulkanVertexBuffer& vertexBuffer) override;
        void cmdBindIndexBuffers(VulkanIndexBuffer& indexBuffer) override;
        void cmdBindDescriptorSets(std::span<VulkanDescriptorSet> descriptorSets, VulkanPipelineLayout& pipelineLayout) override;
        void cmdSetViewport(const VkViewport& viewport) override;
        void cmdSetScissor(const VkRect2D& scissor) override;
        void cmdDraw() override;
        void cmdDrawIndexed(uint32_t indexCount) override;
        void cmdCopyBuffer(const VkBufferCopy& infos, const IVulkanBuffer& src, const IVulkanBuffer& dst);
        void cmdCopyBufferToImage(const IVulkanBuffer& src, const VkImage& dst, const VkBufferImageCopy& infos);

        NARC_GETTER(VkCommandBuffer, getHandle, m_commandBuffer)

        narc_core::result begin() const noexcept override;
        narc_core::result end() const noexcept override;
        narc_core::result reset() const noexcept override;
        narc_core::result copyBuffer(const IBuffer* source, const IBuffer* destination) const noexcept override;
        narc_core::result beginRenderPass(const ISwapchain* swapChain, const RenderPassInfos& infos) const noexcept override;
        narc_core::result endRenderPass() const noexcept override;
        narc_core::result bindPipeline(const IGraphicsPipeline* pipeline) const noexcept override;
        narc_core::result bindViewPort(const ViewPortInfos& viewport) const noexcept override;
        narc_core::result draw() const noexcept override;
        narc_core::result bindScissors(const ScissorsInfos& scissors) const noexcept override;

    private:
        std::shared_ptr<VulkanCommandPool> m_commandPool;
        std::weak_ptr<VulkanDevice> m_device;

        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
    };
} // narc_engine
