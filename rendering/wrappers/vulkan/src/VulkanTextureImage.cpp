//
// Created by theo on 12/22/25.
//

#include "VulkanTextureImage.h"

#include "command/VulkanCommandBuffer.h"
#include "services/VulkanMemoryAllocator.h"

namespace narc_engine {
    VulkanTextureImage::VulkanTextureImage(const VulkanMemoryAllocator* allocator) :
        m_allocator(allocator),
        m_extent()
    {
    }

    VulkanTextureImage::~VulkanTextureImage() = default;

    void VulkanTextureImage::createSampler()
    {
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

    void VulkanTextureImage::init()
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent = {m_extent.Width, m_extent.Height, 1};
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

        createImageView();
        createSampler();
    }

    void VulkanTextureImage::shutdown()
    {
        m_allocator->deallocSampler(m_sampler);
        m_allocator->deallocImageView(m_view);
        m_allocator->deallocImage(m_image, m_allocation);
    }

    void VulkanTextureImage::createImageView()
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
