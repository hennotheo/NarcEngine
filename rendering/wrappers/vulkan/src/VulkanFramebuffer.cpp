//
// Created by theohenno on 11/30/25.
//

#include "VulkanFramebuffer.h"

#include "device/VulkanDevice.h"
#include "pipeline/VulkanRenderPass.h"

namespace narc_engine {
    VulkanFramebuffer::VulkanFramebuffer(std::weak_ptr<VulkanDevice> device, const VulkanSwapChain* swapChain, const VulkanRenderPass* renderPass) :
        m_device(std::move(device)),
        m_swapChain(swapChain),
        m_renderPass(renderPass)
    {
    }

    VulkanFramebuffer::~VulkanFramebuffer() noexcept = default;

    void VulkanFramebuffer::init()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass->getHandle();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = m_attachments.data();
        framebufferInfo.width = m_swapChain->getSwapChainExtent().Width;
        framebufferInfo.height = m_swapChain->getSwapChainExtent().Height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device->getHandle(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create framebuffer!");
        }
    }

    void VulkanFramebuffer::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        
        vkDestroyFramebuffer(device->getHandle(), m_framebuffer, nullptr);
        m_framebuffer = VK_NULL_HANDLE;
    }
} // narc_engine
