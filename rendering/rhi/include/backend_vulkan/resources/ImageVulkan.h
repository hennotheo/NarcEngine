//
// Created by theoh on 7/10/2025.
//

#pragma once

#include "backend_vulkan/device/MemoryAllocatorVulkan.h"
#include "device/MemoryAllocatorRhi.h"
#include "resources/ImageRhi.h"

namespace narc_engine
{
    class SwapChainImageVulkan final : public ImageRhi
    {
    public:
        explicit SwapChainImageVulkan(const VkImage& image) :
            m_image(image)
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

        NARC_GETTER(VkImage, getVkImage, m_image);

    private:
        VkImage m_image = VK_NULL_HANDLE;
    };

    class ImageVulkan final : public ImageRhi
    {
    public:
        BOOST_DI_INJECT(ImageVulkan, const MemoryAllocatorRhiPtr& allocator);
        ~ImageVulkan() override;

        NARC_IMPL_RHI_PLATFORM_GETTER(Image, Vulkan);
        NARC_IMPL_INITIALISABLE();

    private:
        std::weak_ptr<MemoryAllocatorRhi> m_allocator;

        ImageResourceVulkan m_imageResource{};
    };
} // narc_engine
