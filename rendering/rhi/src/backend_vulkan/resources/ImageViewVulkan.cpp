//
// Created by theoh on 7/10/2025.
//

#include "backend_vulkan/resources/ImageViewVulkan.h"

#include "backend_vulkan/resources/ImageVulkan.h"

namespace narc_engine
{
    ImageViewVulkan::ImageViewVulkan(const DeviceRhiPtr& device, const ImageVulkan* image, const VkFormat& format) :
        m_format(format),
        m_device(std::static_pointer_cast<DeviceVulkan>(device)),
        m_image(image)
    {
        if (image == nullptr)
        {
            NARCLOG_FATAL("zzz");
        }

        if (image->getVkImage() == VK_NULL_HANDLE)
        {
            NARCLOG_FATAL("Image is not initialized!");
        }
    }

    ImageViewVulkan::~ImageViewVulkan() = default;

    void ImageViewVulkan::init()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image->getVkImage();
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //TODO: Change this to the correct aspect mask based on the image format
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        vkCreateImageView(device->getVkDevice(), &viewInfo, nullptr, &m_imageView);
    }

    void ImageViewVulkan::shutdown()
    {
        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        vkDestroyImageView(device->getVkDevice(), m_imageView, nullptr);
    }
} // narc_engine
