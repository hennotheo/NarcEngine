//
// Created by theoh on 28/05/2025.
//

#pragma once

#include "resources/ImageRhi.h"
#include "resources/ImageViewRhi.h"
#include "FrameBufferRhi.h"
#include "WindowRhi.h"
#include "device/DeviceRhi.h"

namespace narc_engine
{
    using SwapChainExtends = glm::ivec2;

    NARC_DECL_RHI_PLATFORM_TYPES(SwapChain)

    class NARC_ENGINE_API SwapChainRhi : public narc_core::IInitialisable
    {
    public:
        explicit SwapChainRhi(const WindowRhiPtr& window, const DeviceRhiPtr& device);
        ~SwapChainRhi() override;

        NARC_DECL_RHI_PLATFORM_GETTERS(SwapChain)
        NARC_IMPL_INITIALISABLE();

        NARC_PURE_VIRTUAL_GETTER(SwapChainExtends, getExtends);

        void recreate();

    protected:
        const std::weak_ptr<WindowRhi> m_window;
        const std::weak_ptr<DeviceRhi> m_device;

        std::vector<std::shared_ptr<FrameBufferRhi>> m_framebuffers{};
        std::vector<std::shared_ptr<ImageRhi>> m_images{}; //TODO: Change this to a more specific type later
        std::vector<std::shared_ptr<ImageViewRhi>> m_imageViews{}; //TODO: Change this to a more specific type later

        virtual void createSwapChain() = 0;
        virtual void createImages() = 0;
        virtual void createImageViews() = 0;
        virtual void createFramebuffers() = 0;

        // RhiResult acquireNextImage(const Semaphore* semaphore, uint32_t* imageIndex);

        void cleanup();
        virtual void cleanupSwapChain() = 0;
    };

    using SwapChainRhiPtr = std::shared_ptr<SwapChainRhi>;
} // namespace narc_engine
