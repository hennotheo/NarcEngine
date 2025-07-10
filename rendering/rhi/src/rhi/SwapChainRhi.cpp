//
// Created by theoh on 28/05/2025.
//
#include "SwapchainRhi.h"

namespace narc_engine
{
    SwapChainRhi::SwapChainRhi(const WindowRhiPtr& window, const DeviceRhiPtr& device) :
        m_window(window), m_device(device)
    {

    }

    SwapChainRhi::~SwapChainRhi() = default;

    void SwapChainRhi::recreate()
    {
        NARC_GUARD_WEAK(window, m_window, "Window is null!");

        const auto& size = window->getFramebufferSize();
        if (size.x <= 0 || size.y <= 0)
        {
            NARCLOG_ERROR("SwapChainRhi::recreate() - Invalid framebuffer size: {}x{}", size.x, size.y);
        }

        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        device->waitIdle();

        createSwapChain();
        createImageViews();
        createFramebuffers();
    }

    void SwapChainRhi::createImageViews()
    {
        m_imageViews.resize(m_images.size());

        for (size_t i = 0; i < m_images.size(); i++)
        {
            if (m_imageViews[i] == nullptr)
            {
                m_imageViews[i] = std::make_shared<ImageViewRhi>();
            }

            m_imageViews[i]->init();
        }
    }

    void SwapChainRhi::createFramebuffers()
    {
        m_framebuffers.resize(m_images.size());

        for (size_t i = 0; i < m_framebuffers.size(); i++)
        {
            m_framebuffers[i].init();
        }
    }

    void SwapChainRhi::cleanup()
    {
        std::ranges::for_each(m_images, [](const std::shared_ptr<ImageRhi>& image)
        {
            image->shutdown();
        });

        std::ranges::for_each(m_imageViews, [](const std::shared_ptr<ImageViewRhi>& imageView)
        {
            imageView->shutdown();
        });

        cleanupSwapChain();
    }
} // namespace narc_engine
