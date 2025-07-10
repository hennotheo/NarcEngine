//
// Created by theoh on 7/10/2025.
//

#pragma once

#include "resources/ImageRhi.h"

namespace narc_engine
{
    class ImageVulkan final : public ImageRhi
    {
    public:
        ImageVulkan();
        ~ImageVulkan() override;

        NARC_IMPL_INITIALISABLE();

        static void initVkImagesFromSwapChain(std::vector<std::shared_ptr<ImageRhi>>& images, const VkSwapchainKHR& swapChain,
                                              const VkDevice& device);

    private:
        VkImage m_image = VK_NULL_HANDLE;
    };
} // narc_engine
