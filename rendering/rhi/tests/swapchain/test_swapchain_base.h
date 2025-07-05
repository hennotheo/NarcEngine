//
// Created by theoh on 7/5/2025.
//

#pragma once

#include "test_rhi.h"

using namespace narc_engine;

class RhiSwapChain : public RhiTest
{
public:
    void SetUp() override
    {
        const RhiInjector injector = createRhiInjector(getTestedApi());

        m_context = injector.create<ContextRhiPtr>();
        m_window = injector.create<WindowRhiPtr>();
        m_device = injector.create<DeviceRhiPtr>();
        m_swapChain = injector.create<SwapChainRhiPtr>();

        ASSERT_NE(m_context.get(), nullptr) << "Failed to create ContextRhi for Vulkan API";
        ASSERT_NE(m_window.get(), nullptr) << "Failed to create WindowRhi for Vulkan API";
        ASSERT_NE(m_device.get(), nullptr) << "Failed to create DeviceRhi for Vulkan API";
        ASSERT_NE(m_swapChain.get(), nullptr) << "Failed to create SwapChainRhi for Vulkan API";

        m_context->init();
        m_window->init();
        m_device->init();
    }

    void TearDown() override
    {
        m_device->shutdown();
        m_window->shutdown();
        m_context->shutdown();

        m_swapChain.reset();
        m_device.reset();
        m_window.reset();
        m_context.reset();
    }

protected:
    ContextRhiPtr m_context;
    WindowRhiPtr m_window;
    DeviceRhiPtr m_device;

    SwapChainRhiPtr m_swapChain;
};
