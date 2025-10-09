//
// Created by theoh on 6/29/2025.
//

#pragma once

#include "ContextRhi.h"
#include "device/DeviceRhi.h"
#include "queue/GraphicsQueueRhi.h"
#include "queue/PresentQueueRhi.h"
#include "device/MemoryAllocatorRhi.h"

#include "SwapChainRhi.h"
#include "WindowRhi.h"
#include "FrameBufferRhi.h"


#include "backend_vulkan/ContextVulkan.h"
#include "backend_vulkan/device/DeviceVulkan.h"
#include "backend_vulkan/device/MemoryAllocatorVulkan.h"
#include "backend_vulkan/queue/GraphicsQueueVulkan.h"
#include "backend_vulkan/queue/PresentQueueVulkan.h"

#include "backend_vulkan/WindowVulkan.h"
#include "backend_vulkan/SwapChainVulkan.h"
#include "backend_vulkan/FrameBufferVulkan.h"

namespace narc_engine
{
    inline auto autoCreateVulkanInjector()
    {
        return boost::di::make_injector(boost::di::bind<ContextRhi>().to<ContextVulkan>().in(boost::di::singleton),
                                        boost::di::bind<DeviceRhi>().to<DeviceVulkan>().in(boost::di::singleton),
                                        boost::di::bind<MemoryAllocatorRhi>().to<MemoryAllocatorVulkan>().in(boost::di::singleton),
                                        boost::di::bind<WindowRhi>().to<WindowVulkan>().in(boost::di::singleton), //TODO: Change this to unique later
                                        boost::di::bind<GraphicsQueueRhi>().to<GraphicsQueueVulkan>().in(boost::di::singleton),
                                        boost::di::bind<PresentQueueRhi>().to<PresentQueueVulkan>().in(boost::di::singleton)
                                        // boost::di::bind<SwapChainRhi>().to<SwapChainVulkan>().in(boost::di::unique)
            );
    }

    inline auto createRhiInjector(const RendererApiType apiType)
    {
        switch (apiType)
        {
        case RendererApiType::Vulkan:
            return autoCreateVulkanInjector();

        case RendererApiType::DirectX12:
            NARC_ERROR_RUNTIME("DirectX12 is not supported yet");

        case RendererApiType::OpenGL:
            NARC_ERROR_RUNTIME("OpenGL is not supported yet");

        default:
            NARC_ERROR_RUNTIME("Unsupported API type");
        }
    }

    using RhiInjector = decltype(createRhiInjector(RendererApiType::Vulkan)); // TODO : Update this later
} // namespace narc_engine
