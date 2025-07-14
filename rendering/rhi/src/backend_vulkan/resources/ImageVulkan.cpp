//
// Created by theoh on 7/10/2025.
//

#include "backend_vulkan/resources/ImageVulkan.h"

#include "backend_vulkan/device/MemoryAllocatorVulkan.h"

namespace narc_engine
{

    ImageVulkan::ImageVulkan(const MemoryAllocatorRhiPtr& allocator) :
        m_allocator(allocator)
    {

    }

    ImageVulkan::~ImageVulkan() = default;

    void ImageVulkan::init()
    {
        NARC_GUARD_WEAK(allocator, m_allocator, "Memory allocator is null!");

        VkImageCreateInfo imageCreateInfo;//TODO: temporary replace it by builder pattern
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = nullptr;
        imageCreateInfo.flags = 0;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM; // Example format, change as needed
        imageCreateInfo.extent.width = 1024; // Example width, change as needed
        imageCreateInfo.extent.height = 1024; // Example height, change as needed

        VmaAllocationCreateInfo allocationCreateInfo;
        allocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY; // Example usage, change as needed

        m_imageResource = allocator->getMemoryAllocatorVulkan()->createImage(&imageCreateInfo, &allocationCreateInfo);
    }

    void ImageVulkan::shutdown()
    {
        NARC_GUARD_WEAK(allocator, m_allocator, "Memory allocator is null!");

        allocator->getMemoryAllocatorVulkan()->destroyImage(m_imageResource);
    }
} // narc_engine
