//
// Created by theoh on 7/10/2025.
//

#pragma once

#include "resources/ImageViewRhi.h"

#include "backend_vulkan/device/DeviceVulkan.h"

namespace narc_engine
{
    class ImageVulkan;

    class ImageViewVulkan final : public ImageViewRhi
    {
    public:
        explicit ImageViewVulkan(const DeviceRhiPtr& device, const ImageVulkan* image, const VkFormat& format);
        ~ImageViewVulkan() override;

        ImageViewVulkan(const ImageViewVulkan&) = delete;
        ImageViewVulkan& operator=(const ImageViewVulkan&) = delete;

        NARC_IMPL_RHI_PLATFORM_GETTER(ImageView, Vulkan);
        NARC_IMPL_INITIALISABLE();

        NARC_GETTER(const VkImageView&, getVkImageView, m_imageView);

    private:
        VkImageView m_imageView = VK_NULL_HANDLE;
        const VkFormat m_format;

        std::weak_ptr<DeviceVulkan> m_device;
        const ImageVulkan* m_image;
    };
} // narc_engine
