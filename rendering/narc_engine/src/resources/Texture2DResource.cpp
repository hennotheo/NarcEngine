//
// Created by theoh on 09/03/2025.
//

#include "resources/Texture2DResource.h"

#include <NarcIO.h>
#include <NarcLog.h>

#include "Engine.h"
#include "buffers/StagingBuffer.h"

namespace narc_engine {
    Texture2DResource::Texture2DResource(const char* path, const uint32_t resourceID) : GraphicResource(GraphicResourceType::TEXTURE_2D, resourceID)
    {
        const narc_io::Image image = narc_io::FileReader::readImage(path);

        createTextureImage(image);
        createTextureSampler();
        m_textureImageView.create(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    Texture2DResource::~Texture2DResource()
    {
        const VkDevice device = getDeviceHandler()->getLogicalDevice()->getVkDevice();
        vkDestroySampler(device, m_textureSampler, nullptr);
        m_textureImageView.release();
        vkDestroyImage(device, m_textureImage, nullptr);
        vkFreeMemory(device, m_textureImageMemory, nullptr);
    }

    void Texture2DResource::createTextureImage(const narc_io::Image& sourceImage)
    {
        VkDeviceSize imageSize = sourceImage.getWidth() * sourceImage.getHeight() * 4;

        StagingBuffer staggingBuffer;
        staggingBuffer.create(imageSize);
        staggingBuffer.input(sourceImage.getData());

        Engine::getInstance()->createImage(sourceImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_textureImage, m_textureImageMemory);

        Engine::getInstance()->transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Engine::getInstance()->copyBufferToImage(staggingBuffer.getBuffer(), m_textureImage,
            static_cast<uint32_t>(sourceImage.getWidth()),
            static_cast<uint32_t>(sourceImage.getHeight()));
        Engine::getInstance()->transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_SRGB,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    void Texture2DResource::createTextureSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        VkPhysicalDeviceProperties properties = Engine::getInstance()->getDevice()->getPhysicalDevice()->getPhysicalDeviceProperties();
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(getDeviceHandler()->getLogicalDevice()->getVkDevice(), &samplerInfo, nullptr, &m_textureSampler) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create texture sampler!");
        }
    }
} // narc_engine
