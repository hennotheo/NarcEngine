//
// Created by theohenno on 11/30/25.
//

#pragma once

#include "device/VulkanDevice.h"
#include "pipeline/VulkanRenderPass.h"
#include "swapchain/VulkanSwapChain.h"

namespace narc_engine {
    class VulkanFramebuffer final : public narc_core::IInitialisable
    {
    public:
        explicit VulkanFramebuffer(std::weak_ptr<VulkanDevice> device, const VulkanSwapChain* swapChain,
                          const VulkanRenderPass* renderPass);
        ~VulkanFramebuffer() noexcept override;

        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(VkFramebuffer, getHandle, m_framebuffer);

        void setAttachments(const std::vector<VkImageView>& attachments) { m_attachments = attachments; }
    private:
        std::weak_ptr<VulkanDevice> m_device;
        const VulkanSwapChain* m_swapChain;
        const VulkanRenderPass* m_renderPass;
        
        std::vector<VkImageView> m_attachments;

        VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
    };
} // narc_engine
