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

    // TODO: TEMP TO BE EDITED LATER
    class IRenderPassCmdBuffer
    {
    protected:
        ~IRenderPassCmdBuffer() = default;

    public:
        virtual void cmdBindPipeline(const VulkanGraphicsPipeline& pipeline) = 0;
        virtual void cmdBindVertexBuffers(VulkanVertexBuffer& vertexBuffer) = 0;
        virtual void cmdBindIndexBuffers(VulkanIndexBuffer& indexBuffer) = 0;
        virtual void cmdBindDescriptorSets(std::span<VulkanDescriptorSet> descriptorSets, VulkanPipelineLayout& pipelineLayout) = 0;
        virtual void cmdSetViewport(const VkViewport& viewport) = 0;
        virtual void cmdSetScissor(const VkRect2D& scissor) = 0;
        virtual void cmdDraw() = 0;
        virtual void cmdDrawIndexed(uint32_t indexCount) = 0;
        virtual void endRenderPass() = 0;
    };

    class VulkanCommandBuffer final : public IRenderPassCmdBuffer
    {
    public:
        VkCommandBufferUsageFlags Flags = 0;
        
    public:
        explicit VulkanCommandBuffer(VkCommandBuffer m_commandBuffer);
        ~VulkanCommandBuffer() noexcept;

        void begin();
        void end();

        IRenderPassCmdBuffer* beginRenderPass(const VulkanFramebuffer& framebuffer, const VulkanRenderPass& renderPass);
        void endRenderPass() override;
        
        void cmdBindPipeline(const VulkanGraphicsPipeline& pipeline) override;
        void cmdBindVertexBuffers(VulkanVertexBuffer& vertexBuffer) override;
        void cmdBindIndexBuffers(VulkanIndexBuffer& indexBuffer) override;
        void cmdBindDescriptorSets(std::span<VulkanDescriptorSet> descriptorSets, VulkanPipelineLayout& pipelineLayout) override;
        void cmdSetViewport(const VkViewport& viewport) override;
        void cmdSetScissor(const VkRect2D& scissor) override;
        void cmdDraw() override;
        void cmdDrawIndexed(uint32_t indexCount) override;
        void cmdCopyBuffer(const VkBufferCopy& infos, const IVulkanBuffer& src, const IVulkanBuffer& dst);

        void reset();

        NARC_GETTER(VkCommandBuffer, getHandle, m_commandBuffer);

    private:
        std::shared_ptr<VulkanCommandPool> m_commandPool;
        std::weak_ptr<VulkanDevice> m_device;

        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
    };
} // narc_engine
