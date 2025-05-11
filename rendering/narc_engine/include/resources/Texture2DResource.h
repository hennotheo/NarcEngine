//
// Created by theoh on 09/03/2025.
//

#pragma once

#include <vulkan/vulkan_core.h>

#include "resources/Resource.h"
#include "resources/ImageView.h"

namespace narc_engine
{
    class Texture2DResource final : public Resource
    {
    public:
        explicit Texture2DResource(const char* path);
        ~Texture2DResource() override;

        const VkImageView getImageView() const { return m_textureImageView.get(); }
        const VkSampler& getSampler() const { return m_textureSampler; }

    protected:
        void onLoad() override {};
        void onUnload() override {};
        
    private:
        VkImage m_textureImage;
        VkDeviceMemory m_textureImageMemory;
        ImageView m_textureImageView;
        VkSampler m_textureSampler;

        void createTextureImage(const narc_io::Image& sourceImage);
        void createTextureSampler();
        void createImageTextureView();
    };
} // narc_engine
