//
// Created by theohenno on 11/30/25.
//

#pragma once

namespace narc_engine {
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
        virtual void cmdSetViewport(const VkViewport& viewport) = 0;
        virtual void cmdSetScissor(const VkRect2D& scissor) = 0;
        virtual void cmdDraw() = 0;
        virtual void endRenderPass() = 0;
    };

    class VulkanCommandBuffer final : public narc_core::IInitialisable, public IRenderPassCmdBuffer
    {
    public:
        explicit VulkanCommandBuffer(std::weak_ptr<VulkanDevice> device, std::shared_ptr<VulkanCommandPool> commandPool);
        ~VulkanCommandBuffer() noexcept override;

        NARC_IMPL_INITIALISABLE();

        void begin();
        void end();

        IRenderPassCmdBuffer* beginRenderPass(const VulkanFramebuffer& framebuffer, const VulkanRenderPass& renderPass);
        void endRenderPass() override;
        
        void cmdBindPipeline(const VulkanGraphicsPipeline& pipeline) override;
        void cmdBindVertexBuffers(VulkanVertexBuffer& vertexBuffer) override;
        void cmdSetViewport(const VkViewport& viewport) override;
        void cmdSetScissor(const VkRect2D& scissor) override;
        void cmdDraw() override;

        void reset();

        NARC_GETTER(VkCommandBuffer, getHandle, m_commandBuffer);

    private:
        std::shared_ptr<VulkanCommandPool> m_commandPool;
        std::weak_ptr<VulkanDevice> m_device;

        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
    };
} // narc_engine
