//
// Created by theoh on 28/05/2025.
//
#include "SwapChainRhi.h"

namespace narc_engine {
    SwapChainRhi::SwapChainRhi(const WindowRhi* window, const DeviceRhiPtr& device) : m_window(window), m_device(device) {}

    SwapChainRhi::~SwapChainRhi() = default;

    void SwapChainRhi::init()
    {
        createSwapChain();
        createImages();
        createImageViews();
        createFramebuffers();
    }

    void SwapChainRhi::shutdown() { cleanup(); }

    void SwapChainRhi::recreate()
    {
        NARC_GUARD_RAW_PTR(m_window, "Window is null!");

        const auto& size = m_window->getFramebufferSize();
        if (size.x <= 0 || size.y <= 0)
        {
            NARC_ERROR_RUNTIME("SwapChainRhi::recreate() - Invalid framebuffer size: {}x{}", size.x, size.y);
        }

        NARC_GUARD_WEAK(device, m_device, "Device is null!");

        device->waitIdle();

        createSwapChain();
        createImageViews();
        createFramebuffers();
    }

    void SwapChainRhi::cleanup()
    {
        std::ranges::for_each(m_framebuffers, [](const std::shared_ptr<FrameBufferRhi>& image) { image->shutdown(); });

        std::ranges::for_each(m_imageViews, [](const std::shared_ptr<ImageViewRhi>& imageView) { imageView->shutdown(); });

        cleanupSwapChain();
    }
} // namespace narc_engine
