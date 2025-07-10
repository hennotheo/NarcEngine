//
// Created by theoh on 28/05/2025.
//
#include "SwapchainRhi.h"

namespace narc_engine
{
    SwapChainRhi::SwapChainRhi() = default;

    SwapChainRhi::~SwapChainRhi() = default;

    void SwapChainRhi::createImageViews()
    {
        m_imageViews.resize(m_images.size());

        //TODO: Implement this method properly
        // for (size_t i = 0; i < m_images.size(); i++)
        // {
        //     m_swapChainImageViews[i].create(m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
        // }
    }
} // namespace narc_engine
