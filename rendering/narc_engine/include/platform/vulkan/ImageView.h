#pragma once

#include <vulkan/vulkan_core.h>

#include "core/DeviceComponent.h"

namespace narc_engine
{
    class ImageView : public DeviceComponent, public narc_core::IGetter<VkImageView>
    {
    public:
        NARC_IMPL_IGETTER(VkImageView, m_imageView)

        void create(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void release();

    private:
        VkImageView m_imageView = VK_NULL_HANDLE;
    };
}