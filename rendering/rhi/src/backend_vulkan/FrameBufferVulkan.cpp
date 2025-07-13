//
// Created by theoh on 7/7/2025.
//

#include "backend_vulkan/FrameBufferVulkan.h"

#include "backend_vulkan/SwapChainVulkan.h"
#include "backend_vulkan/device/DeviceVulkan.h"

namespace narc_engine
{
    FrameBufferVulkan::FrameBufferVulkan(const DeviceRhiPtr& device, const SwapChainVulkan* swapChain) :
        m_device(std::static_pointer_cast<DeviceVulkan>(device)),
        m_swapChain(swapChain)
    {
        m_framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    }

    FrameBufferVulkan::~FrameBufferVulkan() = default;

    void FrameBufferVulkan::init()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");
        // NARC_GUARD_WEAK(swapChain, m_swapChain, "SwapChain is null!");

        const SwapChainExtends swapChainExtends = m_swapChain->getExtends();
        const std::vector<const ImageViewRhi*> attachments = getImageViews();

        std::vector<VkImageView> vkAttachments;
        vkAttachments.reserve(attachments.size());
        std::transform(
            attachments.begin(), attachments.end(),
            std::back_inserter(vkAttachments),
            [](const ImageViewRhi* view) { return VK_NULL_HANDLE; }
            );

        //TODO: Fill Renderpass
        // m_framebufferInfo.renderPass = getRenderPass()->get();
        m_framebufferInfo.attachmentCount = static_cast<uint32_t>(vkAttachments.size());
        m_framebufferInfo.pAttachments = vkAttachments.data();
        m_framebufferInfo.width = swapChainExtends.x;
        m_framebufferInfo.height = swapChainExtends.y;
        m_framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device->getVkDevice(), &m_framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create framebuffer!");
        }
    }

    void FrameBufferVulkan::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        vkDestroyFramebuffer(device->getVkDevice(), m_framebuffer, nullptr);
        m_framebuffer = VK_NULL_HANDLE;
    }
} // narc_engine
