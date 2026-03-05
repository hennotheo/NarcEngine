//
// Created by theohenno on 11/28/25.
//

#include "pipeline/VulkanRenderPass.h"

#include "device/VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "mapping/mappingToVk.h"
#include "swapchain/VulkanSwapChain.h"

namespace narc_engine {
    VulkanRenderPass::VulkanRenderPass(const VulkanDevice* device, const VulkanSwapChain* swapChain) :
        m_device(device),
        m_swapChain(swapChain)
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

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkSubpassDependency dependency2{};
        dependency2.srcSubpass = 0;
        dependency2.dstSubpass = VK_SUBPASS_EXTERNAL;
        dependency2.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency2.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency2.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        dependency2.dstAccessMask = 0;

        std::array dependencies{ dependency, dependency2 };
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = dependencies.size();
        renderPassInfo.pDependencies = dependencies.data();

        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");
        if (vkCreateRenderPass(m_device->getHandle(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to create render pass!");
        }
    }

    void VulkanRenderPass::shutdown()
    {
        NARC_GUARD_RAW_PTR(m_device, "Failed to get Vulkan Device.");

        vkDestroyRenderPass(m_device->getHandle(), m_renderPass, nullptr);
        m_renderPass = VK_NULL_HANDLE;
    }

    VkRenderPassBeginInfo VulkanRenderPass::getRenderPassBeginInfo(const VulkanFramebuffer& framebuffer) const
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = framebuffer.getHandle();

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = mapping::mapFromExtend(m_swapChain->getSwapChainExtent());
        
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &m_clearValue;

        return renderPassInfo;
    }
} // narc_engine
