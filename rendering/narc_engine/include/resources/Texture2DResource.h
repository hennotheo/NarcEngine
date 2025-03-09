//
// Created by theoh on 09/03/2025.
//

#pragma once

#include <NarcIO.h>
#include <vulkan/vulkan_core.h>

#include "GraphicResource.h"

namespace narc_engine
{
    class Texture2DResource final : public GraphicResource
    {
    public:
        explicit Texture2DResource(const char* path, const uint32_t resourceID);
        ~Texture2DResource() override;

        const VkImageView& getImageView() const { return m_textureImageView; }
        const VkSampler& getSampler() const { return m_textureSampler; }

    private:
        VkImage m_textureImage;
        VkDeviceMemory m_textureImageMemory;
        VkImageView m_textureImageView;
        VkSampler m_textureSampler;

        void createTextureImage(const narc_io::Image& sourceImage);
        void createTextureSampler();
        void createImageTextureView();
    };
} // narc_engine
