#include "platform/vulkan/ImageView.h"

namespace narc_engine
{
    void ImageView::create(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags; // VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(NARC_DEVICE_HANDLE, &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
        {
            NARCLOG_FATAL("failed to create texture image view!");
        }
    }

    void ImageView::release()
    {
        vkDestroyImageView(NARC_DEVICE_HANDLE, m_imageView, nullptr);
    }
} // namespace narc_engine