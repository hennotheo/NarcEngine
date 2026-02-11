//
// Created by theo on 12/22/25.
//

#include "vulkan_wrappers/VulkanTextureImage.h"

#include "vulkan_wrappers/VulkanCommandBuffer.h"
#include "vulkan_wrappers/VulkanCommandPool.h"
#include "vulkan_wrappers/buffers/VulkanStagingBuffer.h"

namespace narc_engine {
    VulkanTextureImage::VulkanTextureImage(
            NARC_DI_IMPORT_SERVICE(IVulkanMemoryAllocationService), NARC_DI_IMPORT_COMPONENT(VulkanCommandPool),
            NARC_DI_IMPORT_SERVICE(ICmdService)) :
        NARC_DI_IMPL_SERVICE(IVulkanMemoryAllocationService, m_allocator),
        NARC_DI_IMPL_COMPONENT(VulkanCommandPool, m_commandPool),
        NARC_DI_IMPL_SERVICE(ICmdService, m_cmdService)
    {
    }

    VulkanTextureImage::~VulkanTextureImage()
    {
    }

    void VulkanTextureImage::init()
    {
        const auto imageStream = narc_io::FileReaderService::readImage(m_path);

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent = {static_cast<uint32_t>(imageStream->getWidth()), static_cast<uint32_t>(imageStream->getHeight()), 1};
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0; // Optional

        const auto query = m_allocator->allocImage(imageInfo, m_image, m_allocation);
        if (!query.has_value())
        {
            NARC_LOG_FATAL("Vertex buffer allocation failed.");
        }

        m_allocationInfo = query.value();

        VulkanStagingBuffer staging{m_allocator, m_cmdService};
        staging.allocate(m_allocationInfo.size);
        staging.setData(imageStream->getData());

        m_cmdService->doCmdActionAndSubmit([this, staging, &imageStream](auto& cmd) {
            transitionImageLayout(cmd,
                      VK_IMAGE_LAYOUT_UNDEFINED,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      VK_IMAGE_ASPECT_COLOR_BIT,
                      1);

            VkBufferImageCopy region{};
            region.bufferOffset = 0;
            region.bufferRowLength = 0; // tightly packed
            region.bufferImageHeight = 0; // tightly packed
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;
            region.imageOffset = {0, 0, 0};
            region.imageExtent = {static_cast<uint32_t>(imageStream->getWidth()), static_cast<uint32_t>(imageStream->getHeight()), 1};

            cmd.cmdCopyBufferToImage(staging, m_image, region);

            transitionImageLayout(cmd,
                      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                      VK_IMAGE_ASPECT_COLOR_BIT,
                      1);
        });

        staging.deallocate();

        createImageViewCreateInfo();

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        const auto& result = m_allocator->allocSampler(samplerInfo);
        if (!result.has_value())
        {
            NARC_LOG_FATAL("Failed to create texture sampler!");
        }

        m_sampler = result.value();
    }

    void VulkanTextureImage::shutdown()
    {
        m_allocator->deallocSampler(m_sampler);
        m_allocator->deallocImageView(m_view);
        m_allocator->deallocImage(m_image, m_allocation);
    }

    void VulkanTextureImage::transitionImageLayout(const VulkanCommandBuffer& cmd, const VkImageLayout oldLayout, const VkImageLayout newLayout,
                                                   const VkImageAspectFlags aspectMask, const uint32_t mipLevels)
    {
        VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_image;
        barrier.subresourceRange.aspectMask = aspectMask;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            NARC_ERROR_RUNTIME("Transition layout not supported.");
        }

        cmd.cmdPipelineBarrier(srcStage, dstStage, 1, barrier);
    }

    void VulkanTextureImage::createImageViewCreateInfo()
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        const auto result = m_allocator->allocImageView(viewInfo);
        if (!result.has_value())
        {
            NARC_ERROR_RUNTIME("Failed to allocate image view.");
        }

        m_view = result.value();
    }
} // narc_engine
