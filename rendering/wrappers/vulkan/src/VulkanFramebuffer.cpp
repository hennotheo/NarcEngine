//
// Created by theohenno on 11/30/25.
//

#include "VulkanFramebuffer.h"

#include "device/VulkanDevice.h"
#include "pipeline/VulkanRenderPass.h"

namespace narc_engine {
    VulkanFramebuffer::VulkanFramebuffer(const VulkanDevice* device, const VulkanSwapChain* swapChain) :
        m_device(device),
        m_swapChain(swapChain)
    {
    }

    VulkanFramebuffer::~VulkanFramebuffer() noexcept = default;

    void VulkanFramebuffer::init()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass->getHandle();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = m_attachments.data();
        framebufferInfo.width = m_swapChain->getSwapChainExtent().Width;
        framebufferInfo.height = m_swapChain->getSwapChainExtent().Height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device->getHandle(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create framebuffer!");
        }
    }

    void VulkanFramebuffer::shutdown()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");

        vkDestroyFramebuffer(m_device->getHandle(), m_framebuffer, nullptr);
        m_framebuffer = VK_NULL_HANDLE;
    }

    void VulkanFramebuffer::setRenderPass(const VulkanRenderPass* renderPass)
    {
        m_renderPass = renderPass;
    }
} // narc_engine
