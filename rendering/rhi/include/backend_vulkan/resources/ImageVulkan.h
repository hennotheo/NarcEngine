//
// Created by theoh on 7/10/2025.
//

#pragma once

#include "device/MemoryAllocatorRhi.h"
#include "resources/ImageRhi.h"

namespace narc_engine
{
    class ImageVulkan : public ImageRhi
    {
        friend class SwapChainImageVulkan;

    public:
        BOOST_DI_INJECT(ImageVulkan, const MemoryAllocatorRhiPtr& allocator);
        ~ImageVulkan() override;

        NARC_IMPL_RHI_PLATFORM_GETTER(Image, Vulkan);
        NARC_IMPL_INITIALISABLE();


        NARC_GETTER(const VkImage&, getVkImage, m_image);

    protected:
        explicit ImageVulkan(const VkImage& image);

    private:
        std::weak_ptr<MemoryAllocatorRhi> m_allocator;

        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
    };

    class SwapChainImageVulkan final : public ImageVulkan
    {
    public:
        explicit SwapChainImageVulkan(const VkImage& image):
            ImageVulkan(image)
        {
        }

        ~SwapChainImageVulkan() override = default;

        void init() override
        {
            /* No-op, image is already initialized*/
        }

        void shutdown() override
        {
            /* No-op, image is not class dependant */
        }

        NARC_OVERRIDE_GETTER(ImageVulkan*, getImageVulkan, const_cast<SwapChainImageVulkan*>(this));
    };
} // narc_engine
