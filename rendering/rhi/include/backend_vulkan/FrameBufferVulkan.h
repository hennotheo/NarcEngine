//
// Created by theoh on 7/7/2025.
//

#pragma once

#include "device/DeviceRhi.h"
#include "SwapChainRhi.h"
#include "FrameBufferRhi.h"

namespace narc_engine
{
    class FrameBufferVulkan final : public FrameBufferRhi
    {
        using super = FrameBufferRhi;

    public:
        explicit FrameBufferVulkan(const DeviceRhiPtr& device, const SwapChainRhiPtr& swapChain);
        ~FrameBufferVulkan() override;

        NARC_IMPL_INITIALISABLE();

        NARC_IMPL_RHI_PLATFORM_GETTER(FrameBuffer, Vulkan);

    private:
        const std::weak_ptr<DeviceVulkan> m_device;
        const std::weak_ptr<SwapChainRhi> m_swapChain;

        VkFramebufferCreateInfo m_framebufferInfo{};
        VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
    };
} // narc_engine
