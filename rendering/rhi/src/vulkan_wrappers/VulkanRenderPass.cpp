//
// Created by theohenno on 11/28/25.
//

#include "vulkan_wrappers/VulkanRenderPass.h"

#include "vulkan_wrappers/VulkanDevice.h"
#include "vulkan_wrappers/VulkanSwapChain.h"

namespace narc_engine {
    VulkanRenderPass::VulkanRenderPass(const std::unique_ptr<VulkanSwapChain>& swapChain, const std::weak_ptr<VulkanDevice>& device) :
        m_swapChain(swapChain.get()),
        m_device(device)
    {

    }

    VulkanRenderPass::~VulkanRenderPass() = default;

    void VulkanRenderPass::init()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_swapChain->getSwapChainImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        if (vkCreateRenderPass(device->getHandle(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create render pass!");
        }
    }

    void VulkanRenderPass::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");
        
        vkDestroyRenderPass(device->getHandle(), m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }
} // narc_engine
