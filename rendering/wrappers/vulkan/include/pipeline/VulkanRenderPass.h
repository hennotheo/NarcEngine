//
// Created by theohenno on 11/28/25.
//

#pragma once

namespace narc_engine {
    class VulkanFramebuffer;
    class VulkanDevice;
    class VulkanSwapChain;

    class VulkanRenderPass final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanRenderPass(const VulkanDevice* device, const VulkanSwapChain* swapChain);
        ~VulkanRenderPass() override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkRenderPass, getHandle, m_renderPass);

        NO_DISCARD VkRenderPassBeginInfo getRenderPassBeginInfo(const VulkanFramebuffer& framebuffer) const;

    private:
        const VulkanDevice* m_device;
        const VulkanSwapChain* m_swapChain;

        VkClearValue m_clearValue{{{0.0f, 0.0f, 0.0f, 1.0f}}};
        VkRenderPass m_renderPass = VK_NULL_HANDLE;
    };
} // narc_engine
