//
// Created by theoh on 7/10/2025.
//

#include "backend_vulkan/resources/ImageVulkan.h"

namespace narc_engine
{

    ImageVulkan::ImageVulkan() = default;

    ImageVulkan::~ImageVulkan() = default;

    void ImageVulkan::init()
    {

    }

    void ImageVulkan::shutdown()
    {

    }

    void ImageVulkan::initVkImagesFromSwapChain(std::vector<std::shared_ptr<ImageRhi>>& images, const VkSwapchainKHR& swapChain,
                                                const VkDevice& device)
    {
        uint32_t imageCount = 0;
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

        images.resize(imageCount);
        std::vector<VkImage> vkImages(imageCount);
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, vkImages.data());

        for (uint32_t i = 0; i < imageCount; ++i)
        {
            const auto& image = images[i];
            image->getImageVulkan()->m_image = vkImages[i];
        }
    }
} // narc_engine
