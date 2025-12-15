//
// Created by theohenno on 11/30/25.
//

#include "vulkan_wrappers/VulkanCommandBuffer.h"

#include "vulkan_wrappers/VulkanCommandPool.h"
#include "vulkan_wrappers/VulkanDevice.h"
#include "vulkan_wrappers/VulkanGraphicsPipeline.h"
#include "vulkan_wrappers/VulkanRenderPass.h"
#include "vulkan_wrappers/VulkanVertexBuffer.h"

namespace narc_engine {
    VulkanCommandBuffer::VulkanCommandBuffer(std::weak_ptr<VulkanDevice> device, std::shared_ptr<VulkanCommandPool> commandPool) :
        m_commandPool(std::move(commandPool)),
        m_device(std::move(device))
    {
    }

    VulkanCommandBuffer::~VulkanCommandBuffer() noexcept = default;

    void VulkanCommandBuffer::init()
    {
        NARC_GUARD_WEAK(device, m_device, "Failed to get Vulkan Device.");

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool->getHandle();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(device->getHandle(), &allocInfo, &m_commandBuffer) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to allocate command buffers!");
        }
    }

    void VulkanCommandBuffer::shutdown()
    {
        //No action for shutdown
    }

    void VulkanCommandBuffer::begin()
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to begin recording command buffer!");
        }
    }

    void VulkanCommandBuffer::end()
    {
        if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
        {
            NARC_ERROR_RUNTIME("Failed to record command buffer!");
        }
    }

    IRenderPassCmdBuffer* VulkanCommandBuffer::beginRenderPass(const VulkanFramebuffer& framebuffer, const VulkanRenderPass& renderPass)
    {
        const auto infos = renderPass.getRenderPassBeginInfo(framebuffer);
        vkCmdBeginRenderPass(m_commandBuffer, &infos, VK_SUBPASS_CONTENTS_INLINE);

        for (int i = 0; i < infos.clearValueCount; ++i)
        {
            const auto& name = infos.pClearValues[i];
        }

        return this;
    }

    void VulkanCommandBuffer::endRenderPass()
    {
        vkCmdEndRenderPass(m_commandBuffer);
    }

    void VulkanCommandBuffer::cmdBindPipeline(const VulkanGraphicsPipeline& pipeline)
    {
        vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getHandle());
    }

    void VulkanCommandBuffer::cmdBindVertexBuffers(VulkanVertexBuffer& vertexBuffer)
    {
        const std::array vertexBuffers = {vertexBuffer.getHandle()};
        const std::vector<VkDeviceSize> offsets = {0};
        vkCmdBindVertexBuffers(m_commandBuffer, 0, 1, vertexBuffers.data(), offsets.data());
    }

    void VulkanCommandBuffer::cmdSetViewport(const VkViewport& viewport)
    {
        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
    }

    void VulkanCommandBuffer::cmdSetScissor(const VkRect2D& scissor)
    {
        vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
    }

    void VulkanCommandBuffer::cmdDraw()
    {
        vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);
    }

    void VulkanCommandBuffer::reset()
    {
        vkResetCommandBuffer(m_commandBuffer, 0);
    }
} // narc_engine
